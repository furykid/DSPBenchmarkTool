#include "mandelbrot.h"
#include "Enums.h"

int init_DSP(s32 *pDevMat[4], Mandelbrot *m_oMandelbrot)
{
	//init a result matirx
	pHostRstMat = (s32*) malloc(GRID_SIZE * MAT_DIM * MAT_DIM * sizeof(double));

	if (pHostRstMat == NULL) {
		DSPCanRun = False;
		return DSPCanRun;
	}

	memset(pHostRstMat, GRID_SIZE * MAT_DIM * MAT_DIM * sizeof(double), 0);

	//init dasc device
	ret = dascInitDev();
	if(ret != DASC_SUCCESS){
		DSPCanRun = False;
		return DSPCanRun;
	}

	//malloc device memory
	for (i = 0; i < GRID_SIZE; i++) {
		ret = dascMallocConfig((void**) (&pDevMat[i]),
				MAT_DIM * MAT_DIM * sizeof(double), 0, i);
		if (ret != DASC_SUCCESS) {
			DSPCanRun = False;
			return DSPCanRun;
		}
	}

	//set up kernel size
	dascConfigKernel(1, GRID_SIZE, BLOCK_SIZE);

	//load the kernle into DSP
	ret = dascLoadKnl("./dasc_demo.hex", "./dasc_demo.ent");
	if(ret != DASC_SUCCESS){
		DSPCanRun = False;
		return DSPCanRun;
	}

	m_oMandelbrot -> DSPpixel = (u32)(*(u32*)pDevMat[0]);

	ElementValue = zero;
	for(i = 0;i < COL;i++) {
		for(j = 0;j < ROW;j++) {
			if(pDevDstMat[i*ROW+j] != 0) {
				ElementValue = nonZero;
				return DSPCanRun = True;
			}
			else {
				DSPCanRun = False;
			}
		}
	}

	return DSPCanRun;
}

void InitMandelbrot(Mandelbrot *m_oMandelbrot)
{
	// Initialize members
	m_oMandelbrot->max = 4.0;
	m_oMandelbrot->max_iterations = 1000;
	m_oMandelbrot->iteration_target = 997;
	m_oMandelbrot->min_x = -2.0;
	m_oMandelbrot->max_x = 0.8;
	m_oMandelbrot->min_y = -1.2;
	m_oMandelbrot->max_y = 1.2;
	m_oMandelbrot->xtemp = 0.0;
	m_oMandelbrot->target_x = 0.0;
	m_oMandelbrot->target_y = 0.0;
	m_oMandelbrot->zoom = 0.0;
	m_oMandelbrot->flag = 0;
	// rotation vars
	m_oMandelbrot->theta = 0.0;
	m_oMandelbrot -> Xresize = ROW / 2;
	m_oMandelbrot -> Yresize = COL / 2;
}
