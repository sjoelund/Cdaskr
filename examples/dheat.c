/* dheat.f -- translated by f2c (version 20100827).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in \netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/
#include <stdio.h>
#include <stdlib.h>

#include "f2c.h"

#include "../solver/ddaskr_types.h"

/* Table of constant values */

static integer c__1 = 1;

/* ***BEGIN PROLOGUE  DHEAT */
/* ***REFER TO  DDASKR */
/* ***DATE WRITTEN   020815   (YYMMDD) */

/* ***DESCRIPTON */

/* ----------------------------------------------------------------------- */
/* Example program for DDASKR. */
/* DAE system derived from the discretized heat equation on a square. */

/* This is the double precision version. */
/* ----------------------------------------------------------------------- */

/* This program solves a DAE system that arises from the heat equation, */
/*   du/dt = u   + u */
/*            xx    yy */
/* posed on the 2-D unit square with zero Dirichlet boundary conditions. */
/* An M+2 by M+2 mesh is set on the square, with uniform spacing 1/(M+1). */
/* The spatial deriviatives are represented by standard central finite */
/* difference approximations.  At each interior point of the mesh, */
/* the discretized PDE becomes an ODE for the discrete value of u. */
/* At each point on the boundary, we pose the equation u = 0.  The */
/* discrete values of u form a vector U, ordered first by x, then by y. */
/* The result is a DAE system G(t,U,U') = 0 of size NEQ = (M+2)*(M+2). */

/* Initial conditions are posed as u = 16x(1-x)y(1-y) at t = 0. */
/* The problem is solved by DDASKR on the time interval t .le. 10.24. */

/* The root functions are R1(U) = max(u) - 0.1, R2(U) = max(u) - 0.01. */

/* The Krylov linear system solution method, with preconditioning, is */
/* selected.  The preconditioner is a band matrix with half-bandwidths */
/* equal to 1, i.e. a tridiagonal matrix.  (The true half-bandwidths */
/* are equal to M+2.)  This corresponds to ignoring the y-direction */
/* coupling in the ODEs, for purposes of preconditioning.  The extra */
/* iterations resulting from this approximation are offset by the lower */
/* storage and linear system solution costs for a tridiagonal matrix. */

/* The routines DBANJA and DBANPS that generate and solve the banded */
/* preconditioner are provided in a separate file for general use. */

/* The output times are t = .01 * 2**n (n = 0,...,10).  The maximum of */
/* abs(u) over the mesh, and various performance statistics, are printed. */

/* For details and test results on this problem, see the reference: */
/*   Peter N. Brown, Alan C. Hindmarsh, and Linda R. Petzold, */
/*   Using Krylov Methods in the Solution of Large-Scale Differential- */
/*   Algebraic Systems, SIAM J. Sci. Comput., 15 (1994), pp. 1467-1488. */
/* ----------------------------------------------------------------------- */

/* ***ROUTINES CALLED */
/*   UINIT, DDASKR */

/* ***END PROLOGUE  DHEAT */

/* Here are necessary declarations.  The dimension statements use a */
/* maximum value for the mesh parameter M, and assume ML = MU = 1. */

/* Main program */ int MAIN__(void)
{
    /* Format strings */
    static char fmt_30[] = "\002 DHEAT: Heat Equation Example Program for D"
	    "DASKR\002\n\n\002    M+2 by M+2 mesh, M =\002%d\002,  System siz"
	    "e NEQ =\002%d\n\n\002    Root functions are: R1 = max(u) - 0.1"
	    "\002,\002 and R2 = max(u) - 0.01\002\n\n\002    Linear solver meth"
	    "od flag INFO(12) =  %d\002    (0 = direct, 1 = Krylov)\002"
	    "\n    Preconditioner is a banded approximation with ML =  "
	    "%d\002  MU =  %d\n\n\002    Tolerances are RTOL =\002%10.1E"
	    "\002   ATOL =\002%10.1E\n\n";
    static char fmt_40[] = "     t\t\t  UMAX\t      NQ\tH\t STEPS\t   NNI\t   NLI\n";
    static char fmt_60[] = "(e15.5,e12.4,i5,e14.3,i7,i9,i8)";
    static char fmt_60_new[] = "     %8.4E\t  %8.3E    %d\t%4.2E\t  %d\t   %d\t  %d\n";
    static char fmt_61[] = "(20x,\002*****   Root found, JROOT =\002,2i3)";
    static char fmt_65[] = "(//\002 Final time reached =\002,e12.4//)";
    static char fmt_90[] = "(//\002 Final statistics for this run..\002/\002"
	    "   RWORK size =\002,i5,\002   IWORK size =\002,i4/\002   Number "
	    "of time steps ................ =\002,i5/\002   Number of residua"
	    "l evaluations ...... =\002,i5/\002   Number of root function eva"
	    "luations . =\002,i5/\002   Number of preconditioner evaluations "
	    " =\002,i5/\002   Number of preconditioner solves ..... =\002,i5"
	    "/\002   Number of nonlinear iterations ...... =\002,i5/\002   Nu"
	    "mber of linear iterations ......... =\002,i5/\002   Average Kryl"
	    "ov subspace dimension =\002,f8.4/i5,\002 nonlinear conv. failure"
	    "s,\002,i5,\002 linear conv. failures\002)";

    /* System generated locals */
    integer i__1, i__2;
    real_number d__1, d__2, d__3;

    /* Builtin functions */
    integer s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), e_wsfe(void);
    /* Subroutine */ int s_stop(char *, ftnlen);

    /* Local variables */
    static integer i__, m;
    static real_number t, u[144];
    static integer ml;
    static real_number dx, hu;
    static integer mu, nli, neq, nni, npe, nre, liw, nps, lwp, nrt, lrw, nqu, 
	    nst, idid, ncfl, ncfn, info[20], ipar[4];
    static real_number atol;
    extern /* Subroutine */ int resh_();
    static real_number rpar[2];
    static integer nrte;
    static real_number umax;
    static integer liwp;
    static real_number rtol;
    static integer iout, lout, nout;
    static real_number tout;
    static integer mband;
    static real_number coeff, avdim;
    static integer lenpd, msave;
    extern /* Subroutine */ int uinit_(real_number *, real_number *, real_number
	    *, integer *);
    static integer iwork[184], jroot[2];
    static real_number rwork[3373];
    extern /* Subroutine */ int dbanja_(), dbanps_();
    extern /* Subroutine */ int ddaskr_(U_fp, integer *, real_number *,
	    real_number *, real_number *, real_number *, integer *, real_number *,
	     real_number *, integer *, real_number *, integer *, integer *,
	    integer *, real_number *, integer *, U_fp, U_fp, U_fp, integer *,
	    integer *);
    extern /* Subroutine */ int rtheat_();
    static real_number uprime[144];

    /* Fortran I/O blocks */
    static cilist io___40 = { 0, 0, 0, fmt_60, 0 };
    static cilist io___41 = { 0, 6, 0, fmt_61, 0 };
    static cilist io___42 = { 0, 0, 0, fmt_65, 0 };
    static cilist io___50 = { 0, 0, 0, fmt_90, 0 };



/* Here set parameters for the problem being solved.  Use RPAR and IPAR */
/* to communicate these to the other routines. */

    m = 10;
    dx = 1. / (m + 1);
    neq = (m + 2) * (m + 2);
    coeff = 1. / (dx * dx);

    ipar[2] = neq;
    ipar[3] = m;
    rpar[0] = dx;
    rpar[1] = coeff;

/* Here set NRT = number of root functions */
    nrt = 2;

/* Here set the half-bandwidths and load them into IPAR for use by the */
/* preconditioner routines. */
    ml = 1;
    mu = 1;
    ipar[0] = ml;
    ipar[1] = mu;

/* Here set the lengths of the preconditioner work arrays WP and IWP, */
/* load them into IWORK, and set the total lengths of WORK and IWORK. */
    lenpd = ((ml << 1) + mu + 1) * neq;
    mband = ml + mu + 1;
    msave = neq / mband + 1;
    lwp = lenpd + (msave << 1);
    liwp = neq;
    iwork[26] = lwp;
    iwork[27] = liwp;
    lrw = lwp + 2699;
    liw = liwp + 40;

/* Call subroutine UINIT to initialize U and UPRIME. */

    uinit_(u, uprime, rpar, ipar);

/* ----------------------------------------------------------------------- */
/* Here we set up the INFO array, which describes the various options */
/* in the way we want DDASKR to solve the problem. */
/* In this case, we select the iterative preconditioned Krylov method, */
/* and we supply the band preconditioner routines DBANJA/DBANPS. */

/* We first initialize the entire INFO array to zero, then set select */
/* entries to nonzero values for desired solution options. */

/* To select the Krylov iterative method for the linear systems, */
/* we set INFO(12) = 1. */

/* Since we are using a preconditioner that involves approximate */
/* Jacobian elements requiring preprocessing, we have a JAC routine, */
/* namely subroutine DBANJA, and we must set INFO(15) = 1 to indicate */
/* this to DDASKR. */

/* No other entries of INFO need to be changed for this example. */
/* ----------------------------------------------------------------------- */

    for (i__ = 1; i__ <= 20; ++i__) {
/* L10: */
	info[i__ - 1] = 0;
    }

    info[11] = 1;
    info[14] = 1;

/* Here we set tolerances for DDASKR to indicate how much accuracy */
/* we want in the solution, in the sense of local error control. */
/* For this example, we ask for pure absolute error control with a */
/* tolerance of 1.0D-5. */
    rtol = 1e-12;
    atol = 1e-12;

/* Here we generate a heading with important parameter values. */
/* LOUT is the unit number of the output device. */
    printf(fmt_30, m, neq, info[11], ml, mu, rtol, atol);
    printf("%s", fmt_40);

/* ----------------------------------------------------------------------- */
/* Now we solve the problem. */

/* DDASKR will be called to compute 11 intermediate solutions from */
/* tout = 0.01 to tout = 10.24 by powers of 2. */

/* We pass to DDASKR the names DBANJA and DBANPS for the JAC and PSOL */
/* routines to do the preconditioning. */

/* At each output time, we compute and print the max-norm of the */
/* solution (which should decay exponentially in t).  We also print */
/* some relevant statistics -- the current method order and step size, */
/* the number of time steps so far, and the numbers of nonlinear and */
/* linear iterations so far. */

/* If a root was found, we flag this, and return to the DDASKR call. */

/* If DDASKR failed in any way (IDID .lt. 0) we print a message and */
/* stop the integration. */
/* ----------------------------------------------------------------------- */

    nout = 11;
    t = 0.;
    tout = .01;
    i__1 = nout;
    for (iout = 1; iout <= i__1; ++iout) {
L45:
	ddaskr_((U_fp)resh_, &neq, &t, u, uprime, &tout, info, &rtol, &atol, &
		idid, rwork, &lrw, iwork, &liw, rpar, ipar, (U_fp)dbanja_, (
		U_fp)dbanps_, (U_fp)rtheat_, &nrt, jroot);

	umax = 0.;
	i__2 = neq;
	for (i__ = 1; i__ <= i__2; ++i__) {
/* L50: */
/* Computing MAX */
	    d__2 = umax, d__3 = (d__1 = u[i__ - 1], abs(d__1));
	    umax = max(d__2,d__3);
	}

	hu = rwork[6];
	nqu = iwork[7];
	nst = iwork[10];
	nni = iwork[18];
	nli = iwork[19];
	io___40.ciunit = lout;
	s_wsfe(&io___40);
	do_fio(&c__1, (char *)&t, (ftnlen)sizeof(real_number));
	do_fio(&c__1, (char *)&umax, (ftnlen)sizeof(real_number));
	do_fio(&c__1, (char *)&nqu, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&hu, (ftnlen)sizeof(real_number));
	do_fio(&c__1, (char *)&nst, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&nni, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&nli, (ftnlen)sizeof(integer));
	e_wsfe();
	printf(fmt_60_new, t, umax, nqu, hu, nst, nni, nli);

	if (idid == 5) {
	    s_wsfe(&io___41);
	    do_fio(&c__1, (char *)&jroot[0], (ftnlen)sizeof(integer));
	    do_fio(&c__1, (char *)&jroot[1], (ftnlen)sizeof(integer));
	    e_wsfe();
	    goto L45;
	}

	if (idid < 0) {
	    io___42.ciunit = lout;
	    s_wsfe(&io___42);
	    do_fio(&c__1, (char *)&t, (ftnlen)sizeof(real_number));
	    e_wsfe();
	    goto L80;
	}

	tout *= 2.;
/* L70: */
    }

/* Here we display some final statistics for the problem. */
/* The ratio of NLI to NNI is the average dimension of the Krylov */
/* subspace involved in the Krylov linear iterative method. */
L80:
    nst = iwork[10];
    npe = iwork[12];
    nre = iwork[11] + npe * mband;
    liw = iwork[16];
    lrw = iwork[17];
    nni = iwork[18];
    nli = iwork[19];
    nps = iwork[20];
    if (nni != 0) {
	avdim = (real_number) nli / (real_number) nni;
    }
    ncfn = iwork[14];
    ncfl = iwork[15];
    nrte = iwork[35];
    /* printf(fmt_30, &(*i1), &(*i2));*/

    io___50.ciunit = lout;
    s_wsfe(&io___50);
    do_fio(&c__1, (char *)&lrw, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&liw, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&nst, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&nre, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&nrte, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&npe, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&nps, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&nni, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&nli, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&avdim, (ftnlen)sizeof(real_number));
    do_fio(&c__1, (char *)&ncfn, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&ncfl, (ftnlen)sizeof(integer));
    e_wsfe();


/* ------  End of main program for DHEAT example program ----------------- */
    exit(0);
    return 0;
} /* MAIN__ */

/* Subroutine */ int uinit_(real_number *u, real_number *uprime, real_number *
	rpar, integer *ipar)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static integer i__, j, k, m;
    static real_number dx, xj, yk;
    static integer neq, ioff;


/* This routine computes and loads the vector of initial values. */
/* The initial U values are given by the polynomial u = 16x(1-x)y(1-y). */
/* The initial UPRIME values are set to zero.  (DDASKR corrects these */
/* during the first time step.) */


    /* Parameter adjustments */
    --ipar;
    --rpar;
    --uprime;
    --u;

    /* Function Body */
    neq = ipar[3];
    m = ipar[4];
    dx = rpar[1];

    i__1 = m + 1;
    for (k = 0; k <= i__1; ++k) {
	yk = k * dx;
	ioff = (m + 2) * k;
	i__2 = m + 1;
	for (j = 0; j <= i__2; ++j) {
	    xj = j * dx;
	    i__ = ioff + j + 1;
	    u[i__] = xj * 16. * (1. - xj) * yk * (1. - yk);
/* L10: */
	}
/* L20: */
    }
    i__1 = neq;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L30: */
	uprime[i__] = 0.;
    }
    return 0;
/* ------------  End of Subroutine UINIT  -------------------------------- */
} /* uinit_ */

/* Subroutine */ int resh_(real_number *t, real_number *u, real_number *uprime,
	real_number *cj, real_number *delta, integer *ires, real_number *rpar,
	integer *ipar)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static integer i__, j, k, m, m2, neq, ioff;
    static real_number temx, temy, coeff;


/* This is the user-supplied RES subroutine for this example. */
/* It computes the residuals for the 2-D discretized heat equation, */
/* with zero boundary values. */


/* Set problem constants using IPAR and RPAR. */
    /* Parameter adjustments */
    --ipar;
    --rpar;
    --delta;
    --uprime;
    --u;

    /* Function Body */
    neq = ipar[3];
    m = ipar[4];
    coeff = rpar[2];
    m2 = m + 2;

/* Load U into DELTA, in order to set boundary values. */
    i__1 = neq;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L10: */
	delta[i__] = u[i__];
    }

/* Loop over interior points, and load residual values. */
    i__1 = m;
    for (k = 1; k <= i__1; ++k) {
	ioff = m2 * k;
	i__2 = m;
	for (j = 1; j <= i__2; ++j) {
	    i__ = ioff + j + 1;
	    temx = u[i__ - 1] + u[i__ + 1];
	    temy = u[i__ - m2] + u[i__ + m2];
	    delta[i__] = uprime[i__] - (temx + temy - u[i__] * 4.) * coeff;
/* L20: */
	}
/* L30: */
    }

    return 0;
/* ------------  End of Subroutine RESH  --------------------------------- */
} /* resh_ */

/* Subroutine */ int rtheat_(integer *neq, real_number *t, real_number *u,
	real_number *up, integer *nrt, real_number *rval, real_number *rpar,
	integer *ipar)
{
    /* System generated locals */
    integer i__1;
    real_number d__1, d__2;

    /* Local variables */
    static integer i__;
    static real_number umax;


/* This routine finds the max of U, and sets RVAL(1) = max(u) - 0.1, */
/* RVAL(2) = max(u) - 0.01. */


    /* Parameter adjustments */
    --u;
    --rval;

    /* Function Body */
    umax = 0.;
    i__1 = *neq;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L10: */
/* Computing MAX */
	d__1 = umax, d__2 = u[i__];
	umax = max(d__1,d__2);
    }
    rval[1] = umax - .1;
    rval[2] = umax - .01;

    return 0;
/* ------------  End of Subroutine RTHEAT  ------------------------------- */
} /* rtheat_ */

