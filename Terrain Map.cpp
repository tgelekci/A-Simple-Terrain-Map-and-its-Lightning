#include <iostream>
#include <fstream>
#include <string>
#include <glut.h>

using namespace std;
GLfloat xd;											//kulan�lacak noktalar�n x koordinat�
GLfloat zd;											//kulan�lacak noktalar�n z koordinat�
GLfloat yd;											//kulan�lacak noktalar�n y koordinat�
GLint num;											//Read fonksiyonunun parametresi
GLint cx = 0;										//zemine y de�erlerinin eklenmesi s�ras�nda sat�r atlatan de�i�ken
GLint cy;											//zemine x de�erlerinin eklenmesi s�ras�nda sat�r atlatan de�i�ken
GLboolean hcontrol = true;                          //renk de�i�imi i�in kullanc�lacak boolean de�i�keni
GLint angle = 0;									//Zeminin d�n�� de�i�keni
GLdouble spin = 0;                                  //I����n d�n�� de�i�keni
GLfloat pos[] = { 300.0, 0.0, 0.0, 0.5 };			//I��k kayna��n�n koordinatlar�

int array[17][21];
void init(void);          //Init fonksiyonu
void display(void);       //display fonksiyonu
void Read(int sayi);      //.txt dosyas�ndaki de�erleri 2 boyutlu diziye okuyan fonksiyon
void keyboard(unsigned char key, int a, int b);         //klavye fonksiyonu
void Color(int z);         //y�kseltileri renklendiren fonksiyon

void Special_Keys(int key, int x, int y);        //�zel tu� fonksiyonu
void Reshape(int w, int h);                 //yeniden �ekillendirme fonksiyonu(ekran b�y�t�l�rse vb. i�in)
void myidle(void);          //Idle fonksiyonu(���k kayna�� i�in)

void main(int argc, char *argv[])
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(80, 80);
	glutCreateWindow("Project 1");
	init();
	Read(num);
	glutIdleFunc(myidle);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(Special_Keys);
	glutDisplayFunc(display);


	glutMainLoop();


}

void display()
{
	glLoadIdentity();

	gluPerspective(30, 1, 2, 9000);               //alan� g�r�� a��s�-aspect oran�(w/h)-bak�labilen en yak�n ve uzak noktalar
	gluLookAt(-600.0, 800.0, -300.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);   //kamera konumu-kameran�n bakt��� y�n- yukar� vekt�r�


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();												//���k kayna�� i�in 
	
	glRotated(spin, 0.0, 0.0, 1.0);         //�����n d�n�� h�z�n� ayarlar 
	glLightfv(GL_LIGHT0, GL_POSITION, pos);   //���k kayna��n�n parametrelerini ayarlar, GL_POSISITON, pos parametresinin 4 adet veri tutmas�n� sa�lar(koordinatlar)
	glTranslated(250, 0, 0);             //�����n d�n�� �ap�n�(ne kadar geni� d�nece�ini) ayarlar 
	glDisable(GL_LIGHTING);
	glColor3f(0.0, 0.0, 5.0);             //���k kayna��n�n rengi

	glutSolidCube(15);               //���k kayna��n�n �ekli
	glEnable(GL_LIGHTING);           //Bu sat�r kalkarsa ekran hep ayd�nl�k olacakt�r, ���k kayna��n�n anlam� kalmaz
	glEnable(GL_COLOR_MATERIAL);    //Bu sat�r kalkarsa ���k vurunca renkler gider, zemin siyah-beyaz olur.
	glPopMatrix();


	glPolygonMode(GL_FRONT_AND_BACK, GL_POLYGON); //zemin ve ���k kayna��n�n i�ini doldurur, GL_LINE olsayd� a� gibi dururdu.

	glRotated(angle, 0, 1, 0);                //zemini angle a��s� kadar y ekseni etraf�nda d�nd�r�r
	for (zd = -160; zd < 160; zd += 20){

		cy = 0;

		glBegin(GL_TRIANGLE_STRIP);

		for (xd = -200; xd <= 200; xd += 20)              //�lk �nce 20 aral�kl� 17x21'lik bir y�kseltisiz(yd'siz) zemin
		{												  //olu�turuldu. Daha sonra y�kseltiler(yd) eklendi.
														  
			yd = array[cx][cy];                            //.txt'deki y noktalar� bir �stten bir alttan al�narak okunur
			if (hcontrol == true)                          //(14-5-25-18-45-43-.....)
			{											   //y de�erlerinin bulundu�u matriste a�a�� do�ru z, 
														   //sa�a do�ru x de�erleri art�yor
				Color(yd);								   //x ve z'lerin �izim mant��� da y'ler gibidir.
			}                                              //Bir �stten bir alttan al�n�rlar ve TRIANGLE_STRIP ile
			                                               //birle�tirildiklerinde zemini olu�tururlar.
			else
			{
				glColor3f(1.0, 0.0, 1.0);
			}
															//Yani x ve z noktalar�n�n bi�imi s�rayla
			glVertex3f(xd, yd, zd);                         //(x, z)-(x, z+20)-(x+20, z)-(x+20, z+20)-(x+40, z)-(x+40, z+20)-....
															//�eklinde olur. x, 2 ad�mda bir 20 artarken, z bir ad�mda 20 art�p bir ad�mda 20 azalacakt�r
			yd = array[cx + 1][cy];

			if (hcontrol == true)
			{
				Color(yd);
			}

			else
			{
				glColor3f(1.0, 0.0, 1.0);
			}
			
			glVertex3f(xd, yd, zd + 20);                      //cy s�tunu atlat�yor ve cx sat�r� atlat�yor
			
			cy++;
			

		}
		cx++;

		glEnd();

	}


	glutSwapBuffers();

	cx = 0;            //cy ve cx s�f�rlanmazsa sonsuza d�ng�de s�rekli artarlar ve g�r�nt� bozulur. sat�r ve s�tun atlama
	cy = 0;            //i�lemi yap�lamaz

	glFlush();

}
void init()
{

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glLoadIdentity();
	glShadeModel(GLU_SMOOTH);//renkler aras� ge�i�ler yumu�at�ld�

	glMatrixMode(GL_PROJECTION);

	glMatrixMode(GL_MODELVIEW);               //sahnede, objelerde ve ���kta de�i�imler yap�lmas�n� sa�layan
	glEnable(GL_LIGHTING);					  //fonksiyonlar eklendi
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);     //Depth Test, depth buffer ile �al���r



}

void Read(int num)
{
	ifstream ins;
	ins.open("data.txt");
	while ((true))
	{
		for (int k = 0; k < 17; k++)           //.txt dosyas�ndan sat�r okuma
		{
			for (int c = 0; c < 21; c++)      //.txt dosyas�ndan s�tun okuma
			{
				ins >> num;                   //okunan de�erler num de�i�kenine atan�r
				array[k][c] = num;            //num de�i�kenindeki de�erler array'e atan�r
				cout << array[k][c] << " ";
			}

			cout << endl;
		}
		if (ins.eof())						//sat�r sonuna gelindi�inde de�er okuma biter ve d�ng�den ��k�l�r
		{
			break;
		}
	}

}
void Color(int z)                             //e�er y�kseklik 0-50 aras�nda ise ye�il
{											  //50-80 aras�nda ise sar�
	if (yd >= 0 && yd < 50)					  //80'den b�y�k ise k�rm�z� boyan�r
	{
		glColor3f(0.0, 1.0, 0.0);
	}

	else if (yd >= 50 && yd < 80)
	{
		glColor3f(1.0, 1.0, 0.0);           
	}						

	else if (yd >= 80)
	{
		glColor3f(1.0, 0.0, 0.0);
	}
}

void keyboard(unsigned char key, int a, int b)
{
	switch (key)
	{
	case 'h':                           //h veya H'ye bas�ld���nda boolean de�i�keni de�i�ir ve displayde 
	case 'H':							//duruma g�re Color fonksiyonu �a��r�l�r
		hcontrol = !hcontrol;
		break;
	case 'q':                          //q, esc veya Q'ya bas�nca programdan ��k�l�r
	case 'Q':
	case 27:
		exit(EXIT_SUCCESS);

	}

	glutPostRedisplay();

}

void Special_Keys(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT)						//sa� ok tu�una bas�l�nca angle a��s� s�rekli 5'er derece 
	{												//artt�r�larak, zemin sa�a d�nd�r�l�r
		angle -= 5;									//sol ok tu�una bas�l�nca angle a��s� s�rekli 5'er derece
													//azalt�larak, zemin sola d�nd�r�l�r
	}

	else if (key == GLUT_KEY_RIGHT)
	{
		angle += 5;

	}
	glutPostRedisplay();
}

void Reshape(int w, int h)                                    //Ekran�n b�y�t�lmesi vb. durumlar i�in program ayarlan�r
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);            //viewportun alt sol k��esi-viewport geni�li�i ve y�ksekli�i
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void myidle(void)
{
	spin = spin + 0.10;							//spin de�i�keni s�rekli artt�r�larak ���k kayna��n�n d�nmesi sa�lan�r
	glutPostRedisplay();
}

