#include "mandelbrot.h"
#include "Enums.h"

void calculate_mandelbrot(void *data)
{
	if(MandelbrotState == Running || (MandelbrotState == Stopped && FirstPass == true)) {

		FirstPass = false;

		Mandelbrot* oMndt = (Mandelbrot*)data;

		// initialize rotation vars
		oMndt->sine = sin(oMndt->theta);
		oMndt->cosine = cos(oMndt->theta);

		// 35, 17 are great zoom counts
		m_bShouldZoom = m_iCounter < 7 ? TRUE : FALSE;

		//**********************
		// start mandelbrot loop
		//**********************
		omp_set_num_threads(17);

#pragma omp parallel private (i,j,m_iCounter)
		{
#pragma omp sections
			{
#pragma omp section

				for (i = 0; i < ROW; i++) {
					for (j = 0; j < COL; j++) {

						// rotation adjustments
						//******************************************************************************
						i -= oMndt->Xresize;
						j -= oMndt->Yresize;

						oMndt->xRot = ((i * oMndt->cosine) - (j * oMndt->sine));
						oMndt->yRot = ((i * oMndt->sine) + (j * oMndt->cosine));

						oMndt->xRot += oMndt->Xresize;
						oMndt->yRot += oMndt->Yresize;

						i += oMndt->Xresize;
						j += oMndt->Yresize;
						//******************************************************************************

						oMndt->xO = (double)((double)oMndt->yRot / COL) * (oMndt->max_x - oMndt->min_x) + oMndt->min_x;
						oMndt->yO = (double)(((ROW - 1) - (double)oMndt->xRot) / ROW) * (oMndt->max_y - oMndt->min_y) + oMndt->min_y;

						oMndt->x = oMndt->xO;
						oMndt->y = oMndt->yO;

						oMndt->iteration = 0;

						// this is to skip the black sections of the set
						oMndt->p = pow((oMndt->x - .25), 2) + pow(oMndt->y,2);
						if(oMndt->p *(oMndt->p + (oMndt->x - .25)) > (.25 * pow(oMndt->y, 2)) && ((pow((oMndt->x + 1), 2) + pow(oMndt->y, 2)) > .0625)) {

							while(((oMndt->x * oMndt->x) + (oMndt->y * oMndt->y)) <= oMndt->max && oMndt->iteration < oMndt->max_iterations) {

								if(oMndt->iteration > oMndt->iteration_target && m_bShouldZoom) {
									oMndt->target_x = oMndt->xO;
									oMndt->target_y = oMndt->yO;
								}

								oMndt->xtemp = ((oMndt->x * oMndt->x) - (oMndt->y * oMndt->y) + oMndt->xO);
								oMndt->y = (2.0 * oMndt->x * oMndt->y + oMndt->yO);
								oMndt->x = oMndt->xtemp;
								oMndt->iteration++;
							}
						}

						// decide color
						if(oMndt->iteration == oMndt->max_iterations) {
							// in set
							oMndt->color = 0x0;
						}
						else {
							// out of the set
							// color calculation
							oMndt->upper_color = (oMndt->iteration << 8);
							oMndt->mid_color = (oMndt->iteration << 4);
							oMndt->lower_color = (oMndt->iteration << 2);

							// final color combined
							oMndt->color = oMndt->upper_color << 16 | oMndt->mid_color << 8 | oMndt->lower_color;
						}

						// color the pixel
						oMndt -> CPUpixel[i][j] = oMndt->color;
					}
				}
			} /* END OF SECTIONS */

			// Test if the benchmark is running the zoom function
			if(MandelbrotState == Running) {
				oMndt->min_x = oMndt->target_x - 1/(pow(1.5,oMndt->zoom));
				oMndt->max_x = oMndt->target_x + 1/(pow(1.5,oMndt->zoom));
				oMndt->min_y = oMndt->target_y - .75/(pow(1.5,oMndt->zoom));
				oMndt->max_y = oMndt->target_y + .75/(pow(1.5,oMndt->zoom));

				// zoom in the set and increase theta
				oMndt->zoom = zoomIn(oMndt->zoom);
				oMndt->theta = oMndt->theta < 6.28318 ? oMndt->theta += 0.0043625 : 0.0043625;
			}

		} /* END OF MAIN PARALLEL SECTION */
	}
}

double zoomIn(double oZoom) {
	return oZoom + 0.012;
}

