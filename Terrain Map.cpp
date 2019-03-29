#include <iostream>
#include <fstream>
#include <string>
#include <glut.h>

using namespace std;
GLfloat xd;											//kulanýlacak noktalarýn x koordinatý
GLfloat zd;											//kulanýlacak noktalarýn z koordinatý
GLfloat yd;											//kulanýlacak noktalarýn y koordinatý
GLint num;											//Read fonksiyonunun parametresi
GLint cx = 0;										//zemine y deðerlerinin eklenmesi sýrasýnda satýr atlatan deðiþken
GLint cy;											//zemine x deðerlerinin eklenmesi sýrasýnda satýr atlatan deðiþken
GLboolean hcontrol = true;                          //renk deðiþimi için kullancýlacak boolean deðiþkeni
GLint angle = 0;									//Zeminin dönüþ deðiþkeni
GLdouble spin = 0;                                  //Iþýðýn dönüþ deðiþkeni
GLfloat pos[] = { 300.0, 0.0, 0.0, 0.5 };			//Iþýk kaynaðýnýn koordinatlarý

int array[17][21];
void init(void);          //Init fonksiyonu
void display(void);       //display fonksiyonu
void Read(int sayi);      //.txt dosyasýndaki deðerleri 2 boyutlu diziye okuyan fonksiyon
void keyboard(unsigned char key, int a, int b);         //klavye fonksiyonu
void Color(int z);         //yükseltileri renklendiren fonksiyon

void Special_Keys(int key, int x, int y);        //özel tuþ fonksiyonu
void Reshape(int w, int h);                 //yeniden þekillendirme fonksiyonu(ekran büyütülürse vb. için)
void myidle(void);          //Idle fonksiyonu(ýþýk kaynaðý için)

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

	gluPerspective(30, 1, 2, 9000);               //alaný görüþ açýsý-aspect oraný(w/h)-bakýlabilen en yakýn ve uzak noktalar
	gluLookAt(-600.0, 800.0, -300.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);   //kamera konumu-kameranýn baktýðý yön- yukarý vektörü


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();												//ýþýk kaynaðý için 
	
	glRotated(spin, 0.0, 0.0, 1.0);         //ýþýðýn dönüþ hýzýný ayarlar 
	glLightfv(GL_LIGHT0, GL_POSITION, pos);   //ýþýk kaynaðýnýn parametrelerini ayarlar, GL_POSISITON, pos parametresinin 4 adet veri tutmasýný saðlar(koordinatlar)
	glTranslated(250, 0, 0);             //ýþýðýn dönüþ çapýný(ne kadar geniþ döneceðini) ayarlar 
	glDisable(GL_LIGHTING);
	glColor3f(0.0, 0.0, 5.0);             //ýþýk kaynaðýnýn rengi

	glutSolidCube(15);               //ýþýk kaynaðýnýn þekli
	glEnable(GL_LIGHTING);           //Bu satýr kalkarsa ekran hep aydýnlýk olacaktýr, ýþýk kaynaðýnýn anlamý kalmaz
	glEnable(GL_COLOR_MATERIAL);    //Bu satýr kalkarsa ýþýk vurunca renkler gider, zemin siyah-beyaz olur.
	glPopMatrix();


	glPolygonMode(GL_FRONT_AND_BACK, GL_POLYGON); //zemin ve ýþýk kaynaðýnýn içini doldurur, GL_LINE olsaydý að gibi dururdu.

	glRotated(angle, 0, 1, 0);                //zemini angle açýsý kadar y ekseni etrafýnda döndürür
	for (zd = -160; zd < 160; zd += 20){

		cy = 0;

		glBegin(GL_TRIANGLE_STRIP);

		for (xd = -200; xd <= 200; xd += 20)              //Ýlk önce 20 aralýklý 17x21'lik bir yükseltisiz(yd'siz) zemin
		{												  //oluþturuldu. Daha sonra yükseltiler(yd) eklendi.
														  
			yd = array[cx][cy];                            //.txt'deki y noktalarý bir üstten bir alttan alýnarak okunur
			if (hcontrol == true)                          //(14-5-25-18-45-43-.....)
			{											   //y deðerlerinin bulunduðu matriste aþaðý doðru z, 
														   //saða doðru x deðerleri artýyor
				Color(yd);								   //x ve z'lerin çizim mantýðý da y'ler gibidir.
			}                                              //Bir üstten bir alttan alýnýrlar ve TRIANGLE_STRIP ile
			                                               //birleþtirildiklerinde zemini oluþtururlar.
			else
			{
				glColor3f(1.0, 0.0, 1.0);
			}
															//Yani x ve z noktalarýnýn biçimi sýrayla
			glVertex3f(xd, yd, zd);                         //(x, z)-(x, z+20)-(x+20, z)-(x+20, z+20)-(x+40, z)-(x+40, z+20)-....
															//þeklinde olur. x, 2 adýmda bir 20 artarken, z bir adýmda 20 artýp bir adýmda 20 azalacaktýr
			yd = array[cx + 1][cy];

			if (hcontrol == true)
			{
				Color(yd);
			}

			else
			{
				glColor3f(1.0, 0.0, 1.0);
			}
			
			glVertex3f(xd, yd, zd + 20);                      //cy sütunu atlatýyor ve cx satýrý atlatýyor
			
			cy++;
			

		}
		cx++;

		glEnd();

	}


	glutSwapBuffers();

	cx = 0;            //cy ve cx sýfýrlanmazsa sonsuza döngüde sürekli artarlar ve görüntü bozulur. satýr ve sütun atlama
	cy = 0;            //iþlemi yapýlamaz

	glFlush();

}
void init()
{

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glLoadIdentity();
	glShadeModel(GLU_SMOOTH);//renkler arasý geçiþler yumuþatýldý

	glMatrixMode(GL_PROJECTION);

	glMatrixMode(GL_MODELVIEW);               //sahnede, objelerde ve ýþýkta deðiþimler yapýlmasýný saðlayan
	glEnable(GL_LIGHTING);					  //fonksiyonlar eklendi
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);     //Depth Test, depth buffer ile çalýþýr



}

void Read(int num)
{
	ifstream ins;
	ins.open("data.txt");
	while ((true))
	{
		for (int k = 0; k < 17; k++)           //.txt dosyasýndan satýr okuma
		{
			for (int c = 0; c < 21; c++)      //.txt dosyasýndan sütun okuma
			{
				ins >> num;                   //okunan deðerler num deðiþkenine atanýr
				array[k][c] = num;            //num deðiþkenindeki deðerler array'e atanýr
				cout << array[k][c] << " ";
			}

			cout << endl;
		}
		if (ins.eof())						//satýr sonuna gelindiðinde deðer okuma biter ve döngüden çýkýlýr
		{
			break;
		}
	}

}
void Color(int z)                             //eðer yükseklik 0-50 arasýnda ise yeþil
{											  //50-80 arasýnda ise sarý
	if (yd >= 0 && yd < 50)					  //80'den büyük ise kýrmýzý boyanýr
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
	case 'h':                           //h veya H'ye basýldýðýnda boolean deðiþkeni deðiþir ve displayde 
	case 'H':							//duruma göre Color fonksiyonu çaðýrýlýr
		hcontrol = !hcontrol;
		break;
	case 'q':                          //q, esc veya Q'ya basýnca programdan çýkýlýr
	case 'Q':
	case 27:
		exit(EXIT_SUCCESS);

	}

	glutPostRedisplay();

}

void Special_Keys(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT)						//sað ok tuþuna basýlýnca angle açýsý sürekli 5'er derece 
	{												//arttýrýlarak, zemin saða döndürülür
		angle -= 5;									//sol ok tuþuna basýlýnca angle açýsý sürekli 5'er derece
													//azaltýlarak, zemin sola döndürülür
	}

	else if (key == GLUT_KEY_RIGHT)
	{
		angle += 5;

	}
	glutPostRedisplay();
}

void Reshape(int w, int h)                                    //Ekranýn büyütülmesi vb. durumlar için program ayarlanýr
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);            //viewportun alt sol köþesi-viewport geniþliði ve yüksekliði
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void myidle(void)
{
	spin = spin + 0.10;							//spin deðiþkeni sürekli arttýrýlarak ýþýk kaynaðýnýn dönmesi saðlanýr
	glutPostRedisplay();
}

