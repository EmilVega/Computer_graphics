
void Bezier1(){
    int nSegments = 10;
	for (int m = 0; m <= nSegments; m++) {
		GLfloat u = (float)m / (float)nSegments;
		GLfloat Vect[4];
		Vect[0] = u * u*u;
		Vect[1] = u * u;
		Vect[2] = u;
		Vect[3] = 1;
		GLfloat tCoefU[4] = { 0,0,0,0 };
		for (int j = 0; j < 4; j++) {
			GLfloat val = 0;
			for (int i = 0; i < 4; i++) {
				val = val + Vect[i] * Coef[i][j];
			}
			tCoefU[j] = val;
		}
		glBegin(GL_LINE_STRIP);
		for (int k = 0; k <= nSegments; k++) {
			GLfloat v = (float)k / (float)nSegments;
			GLfloat Vect[4];
			Vect[0] = v * v*v;
			Vect[1] = v * v;
			Vect[2] = v;
			Vect[3] = 1;
			GLfloat tCoefV[4] = { 0,0,0,0 };
			for (int j = 0; j < 4; j++) {
				GLfloat val = 0;
				for (int i = 0; i < 4; i++) {
					val = val + Vect[i] * Coef[i][j];
				}
				tCoefV[j] = val;
			}
			GLfloat tCoefP[4][3] = {
				{0,0,0},
				{0,0,0},
				{0,0,0},
				{0,0,0},
			};
			for (int k = 0; k < 4; k++) {
				for (int j = 0; j < 3; j++) {
					GLfloat val = 0;
					for (int i = 0; i < 4; i++) {
						val = val + tCoefV[i] * ctrlpoints[k][i][j];
					}
					tCoefP[k][j] = val;
				}
			}

			GLfloat Punto[3] = { 0,0,0 };
			for (int j = 0; j < 3; j++) {
				GLfloat val = 0;
				for (int i = 0; i < 4; i++) {
					val = val + tCoefU[i] * tCoefP[i][j];
				}
				Punto[j] = val;
			}

			glVertex3fv(Punto);
		}
		glEnd();
	}

	for (int m = 0; m <= nSegments; m++) {
		GLfloat u = (float)m / (float)nSegments;
		GLfloat Vect[4];
		Vect[0] = u * u*u;
		Vect[1] = u * u;
		Vect[2] = u;
		Vect[3] = 1;
		GLfloat tCoefU[4] = { 0,0,0,0 };
		for (int j = 0; j < 4; j++) {
			GLfloat val = 0;
			for (int i = 0; i < 4; i++) {
				val = val + Vect[i] * Coef[i][j];
			}
			tCoefU[j] = val;
		}
		glBegin(GL_LINE_STRIP);
		for (int k = 0; k <= nSegments; k++) {
			GLfloat v = (float)k / (float)nSegments;
			GLfloat Vect[4];
			Vect[0] = v * v*v;
			Vect[1] = v * v;
			Vect[2] = v;
			Vect[3] = 1;
			GLfloat tCoefV[4] = { 0,0,0,0 };
			for (int j = 0; j < 4; j++) {
				GLfloat val = 0;
				for (int i = 0; i < 4; i++) {
					val = val + Vect[i] * Coef[i][j];
				}
				tCoefV[j] = val;
			}
			GLfloat tCoefP[4][3] = {
				{0,0,0},
				{0,0,0},
				{0,0,0},
				{0,0,0},
			};
			for (int k = 0; k < 4; k++) {
				for (int j = 0; j < 3; j++) {
					GLfloat val = 0;
					for (int i = 0; i < 4; i++) {
						val = val + tCoefV[i] * ctrlpoints[i][k][j];
					}
					tCoefP[k][j] = val;
				}
			}

			GLfloat Punto[3] = { 0,0,0 };
			for (int j = 0; j < 3; j++) {
				GLfloat val = 0;
				for (int i = 0; i < 4; i++) {
					val = val + tCoefU[i] * tCoefP[i][j];
				}
				Punto[j] = val;
			}

			glVertex3fv(Punto);
		}
		glEnd();
	}

	
}
