#include <stdio.h>  
#include <stdlib.h>  
#include <math.h>  
#include <glut.h>  


GLUquadricObj* obje = gluNewQuadric();   //Çöp adamýn silindirden oluþan kýsýmlarýnýn çizilmesi için oluþturulan quadric obje.
bool SolTusTik = false;					//Farenin sol tuþuna basýlýp basýlmadýðýný kontrol edecek olan deðiþken. Ayrýca sað alt kolun sallanmasýný denetler.
bool SagTusTik = false;					//Farenin sað tuþuna basýlýp basýlmadýðýný kontrol edecek olan deðiþken  Ayrýca bacaklarýn sallanmasýný ve çöp adamýn yürümesini kontrol eder.
GLdouble sagelAcisi = 180.0;			//Sað alt kolun baþlangýç açýsý. Ayrýca kolu sallama eyleminin geniþliði bu açýnýn büyüklüðü ile belirlenecek.
GLdouble sagelHareket = 0.2;		//Sað alt kolun sallanma hýzý. Ayrýca kolu sallama eyleminde kullanýlacak.
GLdouble solbacHareket = -0.2;		//Sol bacaðýn sallanma hýzý. Ayrýca bacaðý sallama eyleminde kullanýlacak.
GLdouble sagbacHareket = 0.2;		//Sað bacaðýn sallanma hýzý. Ayrýca bacaðý sallama eyleminde kullanýlacak.(Sol bacaðýn deðerinin negatif olmasýnýn sebebi ikisinin ayný anda zýt yönde sallanmalarýdýr.)
GLdouble solbacAcisi = 90.0;		//Sol bacaðýn baþlangýç açýsý. Ayrýca bacaðý sallama eyleminin geniþliði bu açýnýn büyüklüðü ile belirlenecek.
GLdouble sagbacAcisi = 90.0;	//Sað bacaðýn baþlangýç açýsý. Ayrýca bacaðý sallama eyleminin geniþliði bu açýnýn büyüklüðü ile belirlenecek.
GLdouble zHareket = 0.0;		//Çöp adamýn z ekseni boyunca hareket etmesini saðlayan deðiþken
GLdouble kamz = 0.0;			//Kameranýn z ekseni(yatay eksen) boyunca hareket etmesini saðlayan deðiþken
GLdouble kamy = 0.0;			//Kameranýn y ekseni(dikey eksen) boyunca hareket etmesini saðlayan deðiþken
GLdouble followz = 0.0;			//Çöp adam z ekseni boyunca hareket ederken, kameranýn onu takip etmesini saðlayan deðiþken
GLfloat pos[] = { 150.0, 0.0, 0.0, 0.5 };   //Iþýk kaynaðýnýn koordinatlarý


void idle();                                         //idle fonksiyonu(animasyon için)
void mouse(int button, int state, int x, int y);     //fare fonksiyonu(fare iþlemleri için)
void display(void);                                  //display fonksiyonu
void reshape(int w, int h);                          //reshape fonksiyonu(proje tam ekran yapýlýrsa vb. için)
void special_keys(int key, int x, int y);            //özel tuþ fonksiyonu(ok tuþlarý için)


void main(int argc, char *argv[])
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(80, 80);
	glutCreateWindow("Project 2");
	
	glMatrixMode(GL_PROJECTION);    //sahnede, objede ve ýþýk durumunda deðiþiklikler yapýlmasýný saðlayan fonksiyonlar eklendi
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
	
	glPushMatrix();											//ýþýk kaynaðý için 	     
	glLightfv(GL_LIGHT0, GL_POSITION, pos);					//ýþýk kaynaðýnýn parametrelerini ayarlar, GL_POSISITON, pos parametresinin 4 adet veri tutmasýný saðlar(koordinatlar)
	glTranslatef(250.0, 0.0, 0.0);							//ýþýk konumu
	glDisable(GL_LIGHTING);         
             
	glEnable(GL_LIGHTING);						//Bu satýr kalkarsa ekran hep aydýnlýk olacaktýr, ýþýk kaynaðýnýn anlamý kalmaz
	glEnable(GL_COLOR_MATERIAL);				//Bu satýr kalkarsa ýþýk vurunca renkler gider, zemin siyah-beyaz olur.
	glPopMatrix();
	

	gluPerspective(30.0, 1.0, 2.0, 10000.0);               //alaný görüþ açýsý-aspect oraný(w/h)-bakýlabilen en yakýn ve uzak noktalar
	gluLookAt(300.0, 300.0+kamy, 300.0+kamz, 0.0, 0.0, 0.0+followz, 0.0, 1.0, 0.0);   //kamera konumu-kameranýn baktýðý yön- yukarý vektörü
	//gluLookAt fonksiyonuna kamy ve kamz deðerleri eklenerek ok tuþlarýna basýldýðýnda kameranýn eksenler boyunca hareket etmesi saðlanýr.
	
	glBegin(GL_LINES);          
	glColor3d(5, 5, 5);          
	glVertex3d(-10000, 0, 0);      
	glVertex3d(10000, 0, 0);        
	glColor3d(5, 5, 0);              
	glVertex3d(0, -10000, 0);        //Proje yapýlýrken daha iyi görme amaçlý koordinat sistemi çizilmiþtir. 
	glVertex3d(0, 10000, 0);         //glBegin(GL_LINES) ve glEnd(); arasýný kapsamaktadýr.
	glColor3d(1, 0, 2);             //Ýsteðe göre yoruma alýnabilir.
	glVertex3d(0, 0, -10000);      //z ekseni mor, y ekseni sarý, x ekseni beyazdýr.
	glVertex3d(0, 0, 10000);      
	glEnd();                     

	

	glPushMatrix();                     //ANA GÖVDE
	glColor3d(0, 1, 1);
	gluQuadricDrawStyle(obje, GLU_FILL);       //GLU_FILL yerine GLU_LINE yazýlýrsa silindir að þeklinde görünecektir.
	glTranslatef(0.0, 15.0, zHareket);         //Ana gövde, alt ve üst kollar, bacaklar ve boyun silindirle oluþturuldu
	glRotatef(90.0, 1.0, 0.0, 0.0);
	gluCylinder(obje,15,15,40,30,30);         //en baþta tanýmlanan obje aracýlýðýyla silindir oluþturma
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

	glPushMatrix();                     //SAÐ BACAK
	gluQuadricDrawStyle(obje, GLU_FILL);
	glTranslatef(4.0, -16.0, zHareket);
	glRotatef(15.0, 0.0, 0.0, 1.0);
	glRotatef(sagbacAcisi, 1.0, 0.0, 0.0);
	glColor3d(0, 0, 1);
	gluCylinder(obje, 2, 2, 45, 30, 30);
	glPopMatrix();

	glPushMatrix();                       //SOL ÜST KOL
	glColor3d(0, 0, 1);
	gluQuadricDrawStyle(obje, GLU_FILL);
	glTranslatef(-9.0, 7.0, zHareket);
	glRotatef(-60.0, 0.0, 0.0, 1.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	gluCylinder(obje, 2, 2, 23, 30, 30);
	glPopMatrix();

	glPushMatrix();                       //SAÐ ÜST KOL
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

	glPushMatrix();                               //SAÐ ALT KOL
	glColor3d(1, 1, 0);
	gluQuadricDrawStyle(obje, GLU_FILL);
	glTranslatef(31.0, 7.0, zHareket);
	glRotatef(sagelAcisi, 0.0, 0.0, 1.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	gluCylinder(obje, 2, 2, 23, 30, 30);
	glPopMatrix();



	glPushMatrix();            //ANA GÖVDE-SOL BACAK BAÐLANTISI 
	glColor3d(1, 0, 0);
	glTranslatef(-4.0, -16.0, zHareket);
	glutSolidSphere(2, 30, 30);
	glPopMatrix();	


	glPushMatrix();            //ANA GÖVDE-SAÐ BACAK BAÐLANTISI 
	glColor3d(1, 0, 0);
	glTranslatef(4.0, -16.0, zHareket);            //Baðlantýlar(eklemler) ve kafa kürelerle oluþturuldu
	glutSolidSphere(2, 30, 30);
	glPopMatrix();


	glPushMatrix();            //ANA GÖVDE-SAÐ KOL BAÐLANTISI 
	glColor3d(1, 0, 0);
	glTranslatef(9.0, 7.0, zHareket);
	glutSolidSphere(2, 30, 30);
	glPopMatrix();

	glPushMatrix();            //ANA GÖVDE-SOL KOL BAÐLANTISI 
	glColor3d(1, 0, 0);
	glTranslatef(-9.0, 7.0, zHareket);
	glutSolidSphere(2, 30, 30);
	glPopMatrix();

	glPushMatrix();                    //SOL KOLLAR ARASI BAÐLANTI
	glColor3d(1, 0, 0);
	glTranslatef(-28.0, -4.0, zHareket);
	glutSolidSphere(2, 30, 30);
	glPopMatrix();

	glPushMatrix();                 //SAÐ KOLLAR ARASI BAÐLANTI
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
		SolTusTik = !SolTusTik;                               //Sol fare tuþuna basýlýnca SolTusTik deðiþkeni tersine eþitlenir.
		
	}                                                   
	else if (button == GLUT_RIGHT_BUTTON&&state == GLUT_DOWN)
	{
		SagTusTik = !SagTusTik;                         //Sað fare tuþuna basýlýnca SagTusTik deðiþkeni tersine eþitlenir ve followz deðiþkeni arttýrýlarak
		
		
	}
	
}

void idle()
{
	if (SolTusTik == true)                //SolTusTik deðiþkeni baþta false olarak tanýmlanmýþtý. Farenin sol tuþuna týklandýðýnda
	{                                     //mouse fonksiyonu devreye girer ve deðerini true yapar. Çöp adam sað kolunu sallamaya baþlar.
	                                      //sagElAcisi belli deðerlere ulaþtýðýnda(if lerin içindeki deðerler) kol yön deðiþtirir ve sallama hareketi gerçekleþir.
		if (sagelAcisi >= 225.0)          //Farenin sol tuþuna 2. kere týklanýnca SolTusTik deðiþkeni false olacak ve sallanan kol en son olduðu pozisyonda duracaktýr.
		{
			sagelHareket = -sagelHareket;
		}
		else if (sagelAcisi <= 135.0)
		{
			sagelHareket = -sagelHareket;
		}
		sagelAcisi = sagelAcisi + sagelHareket;       //sagelAcisi deðiþkeninin hesaplanmasý. sagelHareket deðiþkeni þartlara göre eklenerek kolun sallanacaðý yönün ayarlanmasýnda rol oynar.
	}

	if (SagTusTik == true)								//SagTusTik deðiþkeni baþta false olarak tanýmlanmýþtý. Farenin sað tuþuna týklandýðýnda                            
	{													//mouse fonksiyonu devreye girer ve deðerini true yapar. Çöp adam bacaklarýný sallamaya baþlar.					
		if (solbacAcisi >= 110.0)
		{
			solbacHareket = -solbacHareket;				//sagbacAcisi ve solbacAcisi belli deðerlere ulaþtýðýnda(if lerin içindeki deðerler) bacaklar yön deðiþtirir ve sallama hareketi gerçekleþir.

		}													//Farenin sað tuþuna 2. kere týklanýnca SagTusTik deðiþkeni false olacak ve sallanan bacaklar en son olduklarý pozisyonlarda duracaklardýr.		
		else if (solbacAcisi <= 70.0)
		{
			solbacHareket = -solbacHareket;
		}
		solbacAcisi = solbacAcisi + solbacHareket;          //solbacAcisi deðiþkeninin hesaplanmasý. solbacHareket deðiþkeni þartlara göre eklenerek sol bacaðýn sallanacaðý yönün ayarlanmasýnda rol oynar.

		if (sagbacAcisi>=110.0)
		{
			sagbacHareket = -sagbacHareket;
		}
		else if (sagbacAcisi<=70.0)
		{
			sagbacHareket = -sagbacHareket;
		}
		sagbacAcisi = sagbacAcisi + sagbacHareket;			//sagbacAcisi deðiþkeninin hesaplanmasý. sagbacHareket deðiþkeni þartlara göre eklenerek sað bacaðýn sallanacaðý yönün ayarlanmasýnda rol oynar.			
		zHareket += 0.1;								//zHareket deðiþkeni arttýrýlarak, çöp adamýn z ekseni boyunca ilerlemesi saðlanýr.
		followz += 0.1;									//followz deðiþkeni arttýrýlarak, çöp adam z ekseni boyunca ilerlerken, kameranýn da onu takip etmesi saðlanýr.
	}
	
	glutPostRedisplay();

}

void reshape(int w, int h)                                    //Ekranýn büyütülmesi vb. durumlar için program ayarlanýr
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);           //viewportun alt sol köþesi-viewport geniþliði ve yüksekliði
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
		kamy -= 20.0;                      //Aþaðý ve yukarý ok tuþlarýna basýlýnca kamy deðeri deðiþtirilerek, kamera dik eksen boyunca hareket ettirilir.(y ekseni)
	}

	if (key == GLUT_KEY_LEFT)             //Sol ve sað ok tuþlarýna basýlýnca kamz deðeri deðiþtirilerek, kamera yatay eksen boyunca hareket ettirilir.(z ekseni)
	{
		kamz -= 20.0;
	}
	
	 if (key==GLUT_KEY_RIGHT)
	{
		kamz += 20.0;
	}
}