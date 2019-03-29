#include <stdio.h>  
#include <stdlib.h>  
#include <math.h>  
#include <glut.h>  


GLUquadricObj* obje = gluNewQuadric();   //��p adam�n silindirden olu�an k�s�mlar�n�n �izilmesi i�in olu�turulan quadric obje.
bool SolTusTik = false;					//Farenin sol tu�una bas�l�p bas�lmad���n� kontrol edecek olan de�i�ken. Ayr�ca sa� alt kolun sallanmas�n� denetler.
bool SagTusTik = false;					//Farenin sa� tu�una bas�l�p bas�lmad���n� kontrol edecek olan de�i�ken  Ayr�ca bacaklar�n sallanmas�n� ve ��p adam�n y�r�mesini kontrol eder.
GLdouble sagelAcisi = 180.0;			//Sa� alt kolun ba�lang�� a��s�. Ayr�ca kolu sallama eyleminin geni�li�i bu a��n�n b�y�kl��� ile belirlenecek.
GLdouble sagelHareket = 0.2;		//Sa� alt kolun sallanma h�z�. Ayr�ca kolu sallama eyleminde kullan�lacak.
GLdouble solbacHareket = -0.2;		//Sol baca��n sallanma h�z�. Ayr�ca baca�� sallama eyleminde kullan�lacak.
GLdouble sagbacHareket = 0.2;		//Sa� baca��n sallanma h�z�. Ayr�ca baca�� sallama eyleminde kullan�lacak.(Sol baca��n de�erinin negatif olmas�n�n sebebi ikisinin ayn� anda z�t y�nde sallanmalar�d�r.)
GLdouble solbacAcisi = 90.0;		//Sol baca��n ba�lang�� a��s�. Ayr�ca baca�� sallama eyleminin geni�li�i bu a��n�n b�y�kl��� ile belirlenecek.
GLdouble sagbacAcisi = 90.0;	//Sa� baca��n ba�lang�� a��s�. Ayr�ca baca�� sallama eyleminin geni�li�i bu a��n�n b�y�kl��� ile belirlenecek.
GLdouble zHareket = 0.0;		//��p adam�n z ekseni boyunca hareket etmesini sa�layan de�i�ken
GLdouble kamz = 0.0;			//Kameran�n z ekseni(yatay eksen) boyunca hareket etmesini sa�layan de�i�ken
GLdouble kamy = 0.0;			//Kameran�n y ekseni(dikey eksen) boyunca hareket etmesini sa�layan de�i�ken
GLdouble followz = 0.0;			//��p adam z ekseni boyunca hareket ederken, kameran�n onu takip etmesini sa�layan de�i�ken
GLfloat pos[] = { 150.0, 0.0, 0.0, 0.5 };   //I��k kayna��n�n koordinatlar�


void idle();                                         //idle fonksiyonu(animasyon i�in)
void mouse(int button, int state, int x, int y);     //fare fonksiyonu(fare i�lemleri i�in)
void display(void);                                  //display fonksiyonu
void reshape(int w, int h);                          //reshape fonksiyonu(proje tam ekran yap�l�rsa vb. i�in)
void special_keys(int key, int x, int y);            //�zel tu� fonksiyonu(ok tu�lar� i�in)


void main(int argc, char *argv[])
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(80, 80);
	glutCreateWindow("Project 2");
	
	glMatrixMode(GL_PROJECTION);    //sahnede, objede ve ���k durumunda de�i�iklikler yap�lmas�n� sa�layan fonksiyonlar eklendi
	glMatrixMode(GL_MODELVIEW);
	glShadeModel(GLU_SMOOTH);
	glEnable(GL_LIGHTING);					  
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);     

	
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutSpecialFunc(special_keys);
	glutDisplayFunc(display);
	
	

	glutMainLoop();


}

void display()
{

	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();											//���k kayna�� i�in 	     
	glLightfv(GL_LIGHT0, GL_POSITION, pos);					//���k kayna��n�n parametrelerini ayarlar, GL_POSISITON, pos parametresinin 4 adet veri tutmas�n� sa�lar(koordinatlar)
	glTranslatef(250.0, 0.0, 0.0);							//���k konumu
	glDisable(GL_LIGHTING);         
             
	glEnable(GL_LIGHTING);						//Bu sat�r kalkarsa ekran hep ayd�nl�k olacakt�r, ���k kayna��n�n anlam� kalmaz
	glEnable(GL_COLOR_MATERIAL);				//Bu sat�r kalkarsa ���k vurunca renkler gider, zemin siyah-beyaz olur.
	glPopMatrix();
	

	gluPerspective(30.0, 1.0, 2.0, 10000.0);               //alan� g�r�� a��s�-aspect oran�(w/h)-bak�labilen en yak�n ve uzak noktalar
	gluLookAt(300.0, 300.0+kamy, 300.0+kamz, 0.0, 0.0, 0.0+followz, 0.0, 1.0, 0.0);   //kamera konumu-kameran�n bakt��� y�n- yukar� vekt�r�
	//gluLookAt fonksiyonuna kamy ve kamz de�erleri eklenerek ok tu�lar�na bas�ld���nda kameran�n eksenler boyunca hareket etmesi sa�lan�r.
	
	glBegin(GL_LINES);          
	glColor3d(5, 5, 5);          
	glVertex3d(-10000, 0, 0);      
	glVertex3d(10000, 0, 0);        
	glColor3d(5, 5, 0);              
	glVertex3d(0, -10000, 0);        //Proje yap�l�rken daha iyi g�rme ama�l� koordinat sistemi �izilmi�tir. 
	glVertex3d(0, 10000, 0);         //glBegin(GL_LINES) ve glEnd(); aras�n� kapsamaktad�r.
	glColor3d(1, 0, 2);             //�ste�e g�re yoruma al�nabilir.
	glVertex3d(0, 0, -10000);      //z ekseni mor, y ekseni sar�, x ekseni beyazd�r.
	glVertex3d(0, 0, 10000);      
	glEnd();                     

	

	glPushMatrix();                     //ANA G�VDE
	glColor3d(0, 1, 1);
	gluQuadricDrawStyle(obje, GLU_FILL);       //GLU_FILL yerine GLU_LINE yaz�l�rsa silindir a� �eklinde g�r�necektir.
	glTranslatef(0.0, 15.0, zHareket);         //Ana g�vde, alt ve �st kollar, bacaklar ve boyun silindirle olu�turuldu
	glRotatef(90.0, 1.0, 0.0, 0.0);
	gluCylinder(obje,15,15,40,30,30);         //en ba�ta tan�mlanan obje arac�l���yla silindir olu�turma
	glPopMatrix();
	
	glPushMatrix();                    //BOYUN
	glColor3f(1, 0, 0);
	gluQuadricDrawStyle(obje, GLU_FILL);
	glTranslatef(0.0, 21.0, zHareket);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	gluCylinder(obje, 3, 3, 6, 30, 30);
	glPopMatrix();

	glPushMatrix();                    //KAFA
	glColor3d(0, 1, 0);
	glTranslatef(0.0, 32.0, zHareket);
	glutSolidSphere(12, 30, 30);
	glPopMatrix();

	glPushMatrix();                     //SOL BACAK
	gluQuadricDrawStyle(obje, GLU_FILL);
	glTranslatef(-4.0, -16.0, zHareket);
	glRotatef(-15.0, 0.0, 0.0, 1.0);
	glRotatef(solbacAcisi, 1.0, 0.0, 0.0);
	glColor3d(0, 0, 1);
	gluCylinder(obje, 2, 2, 45, 30, 30);
	glPopMatrix();

	glPushMatrix();                     //SA� BACAK
	gluQuadricDrawStyle(obje, GLU_FILL);
	glTranslatef(4.0, -16.0, zHareket);
	glRotatef(15.0, 0.0, 0.0, 1.0);
	glRotatef(sagbacAcisi, 1.0, 0.0, 0.0);
	glColor3d(0, 0, 1);
	gluCylinder(obje, 2, 2, 45, 30, 30);
	glPopMatrix();

	glPushMatrix();                       //SOL �ST KOL
	glColor3d(0, 0, 1);
	gluQuadricDrawStyle(obje, GLU_FILL);
	glTranslatef(-9.0, 7.0, zHareket);
	glRotatef(-60.0, 0.0, 0.0, 1.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	gluCylinder(obje, 2, 2, 23, 30, 30);
	glPopMatrix();

	glPushMatrix();                       //SA� �ST KOL
	glColor3d(0, 0, 1);
	gluQuadricDrawStyle(obje, GLU_FILL);
	glTranslatef(9.0, 7.0, zHareket);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	gluCylinder(obje, 2, 2, 23, 30, 30);
	glPopMatrix();

	glPushMatrix();                               //SOL ALT KOL
	glColor3d(1, 1, 0);
	gluQuadricDrawStyle(obje, GLU_FILL);
	glTranslatef(-28.0, -4.0, zHareket);
	glRotatef(-150.0, 0.0, 0.0, 1.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	gluCylinder(obje, 2, 2, 23, 30, 30);
	glPopMatrix();

	glPushMatrix();                               //SA� ALT KOL
	glColor3d(1, 1, 0);
	gluQuadricDrawStyle(obje, GLU_FILL);
	glTranslatef(31.0, 7.0, zHareket);
	glRotatef(sagelAcisi, 0.0, 0.0, 1.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	gluCylinder(obje, 2, 2, 23, 30, 30);
	glPopMatrix();



	glPushMatrix();            //ANA G�VDE-SOL BACAK BA�LANTISI 
	glColor3d(1, 0, 0);
	glTranslatef(-4.0, -16.0, zHareket);
	glutSolidSphere(2, 30, 30);
	glPopMatrix();	


	glPushMatrix();            //ANA G�VDE-SA� BACAK BA�LANTISI 
	glColor3d(1, 0, 0);
	glTranslatef(4.0, -16.0, zHareket);            //Ba�lant�lar(eklemler) ve kafa k�relerle olu�turuldu
	glutSolidSphere(2, 30, 30);
	glPopMatrix();


	glPushMatrix();            //ANA G�VDE-SA� KOL BA�LANTISI 
	glColor3d(1, 0, 0);
	glTranslatef(9.0, 7.0, zHareket);
	glutSolidSphere(2, 30, 30);
	glPopMatrix();

	glPushMatrix();            //ANA G�VDE-SOL KOL BA�LANTISI 
	glColor3d(1, 0, 0);
	glTranslatef(-9.0, 7.0, zHareket);
	glutSolidSphere(2, 30, 30);
	glPopMatrix();

	glPushMatrix();                    //SOL KOLLAR ARASI BA�LANTI
	glColor3d(1, 0, 0);
	glTranslatef(-28.0, -4.0, zHareket);
	glutSolidSphere(2, 30, 30);
	glPopMatrix();

	glPushMatrix();                 //SA� KOLLAR ARASI BA�LANTI
	glColor3d(1, 0, 0);
	glTranslatef(31.0, 7.0, zHareket);
	glutSolidSphere(2, 30, 30);
	glPopMatrix();

	

	glutSwapBuffers();

}

void mouse(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN)
	{
		SolTusTik = !SolTusTik;                               //Sol fare tu�una bas�l�nca SolTusTik de�i�keni tersine e�itlenir.
		
	}                                                   
	else if (button == GLUT_RIGHT_BUTTON&&state == GLUT_DOWN)
	{
		SagTusTik = !SagTusTik;                         //Sa� fare tu�una bas�l�nca SagTusTik de�i�keni tersine e�itlenir ve followz de�i�keni artt�r�larak
		
		
	}
	
}

void idle()
{
	if (SolTusTik == true)                //SolTusTik de�i�keni ba�ta false olarak tan�mlanm��t�. Farenin sol tu�una t�kland���nda
	{                                     //mouse fonksiyonu devreye girer ve de�erini true yapar. ��p adam sa� kolunu sallamaya ba�lar.
	                                      //sagElAcisi belli de�erlere ula�t���nda(if lerin i�indeki de�erler) kol y�n de�i�tirir ve sallama hareketi ger�ekle�ir.
		if (sagelAcisi >= 225.0)          //Farenin sol tu�una 2. kere t�klan�nca SolTusTik de�i�keni false olacak ve sallanan kol en son oldu�u pozisyonda duracakt�r.
		{
			sagelHareket = -sagelHareket;
		}
		else if (sagelAcisi <= 135.0)
		{
			sagelHareket = -sagelHareket;
		}
		sagelAcisi = sagelAcisi + sagelHareket;       //sagelAcisi de�i�keninin hesaplanmas�. sagelHareket de�i�keni �artlara g�re eklenerek kolun sallanaca�� y�n�n ayarlanmas�nda rol oynar.
	}

	if (SagTusTik == true)								//SagTusTik de�i�keni ba�ta false olarak tan�mlanm��t�. Farenin sa� tu�una t�kland���nda                            
	{													//mouse fonksiyonu devreye girer ve de�erini true yapar. ��p adam bacaklar�n� sallamaya ba�lar.					
		if (solbacAcisi >= 110.0)
		{
			solbacHareket = -solbacHareket;				//sagbacAcisi ve solbacAcisi belli de�erlere ula�t���nda(if lerin i�indeki de�erler) bacaklar y�n de�i�tirir ve sallama hareketi ger�ekle�ir.

		}													//Farenin sa� tu�una 2. kere t�klan�nca SagTusTik de�i�keni false olacak ve sallanan bacaklar en son olduklar� pozisyonlarda duracaklard�r.		
		else if (solbacAcisi <= 70.0)
		{
			solbacHareket = -solbacHareket;
		}
		solbacAcisi = solbacAcisi + solbacHareket;          //solbacAcisi de�i�keninin hesaplanmas�. solbacHareket de�i�keni �artlara g�re eklenerek sol baca��n sallanaca�� y�n�n ayarlanmas�nda rol oynar.

		if (sagbacAcisi>=110.0)
		{
			sagbacHareket = -sagbacHareket;
		}
		else if (sagbacAcisi<=70.0)
		{
			sagbacHareket = -sagbacHareket;
		}
		sagbacAcisi = sagbacAcisi + sagbacHareket;			//sagbacAcisi de�i�keninin hesaplanmas�. sagbacHareket de�i�keni �artlara g�re eklenerek sa� baca��n sallanaca�� y�n�n ayarlanmas�nda rol oynar.			
		zHareket += 0.1;								//zHareket de�i�keni artt�r�larak, ��p adam�n z ekseni boyunca ilerlemesi sa�lan�r.
		followz += 0.1;									//followz de�i�keni artt�r�larak, ��p adam z ekseni boyunca ilerlerken, kameran�n da onu takip etmesi sa�lan�r.
	}
	
	glutPostRedisplay();

}

void reshape(int w, int h)                                    //Ekran�n b�y�t�lmesi vb. durumlar i�in program ayarlan�r
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);           //viewportun alt sol k��esi-viewport geni�li�i ve y�ksekli�i
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void special_keys(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
	{
		kamy += 20.0;
	}

	 if (key==GLUT_KEY_DOWN)
	{
		kamy -= 20.0;                      //A�a�� ve yukar� ok tu�lar�na bas�l�nca kamy de�eri de�i�tirilerek, kamera dik eksen boyunca hareket ettirilir.(y ekseni)
	}

	if (key == GLUT_KEY_LEFT)             //Sol ve sa� ok tu�lar�na bas�l�nca kamz de�eri de�i�tirilerek, kamera yatay eksen boyunca hareket ettirilir.(z ekseni)
	{
		kamz -= 20.0;
	}
	
	 if (key==GLUT_KEY_RIGHT)
	{
		kamz += 20.0;
	}
}