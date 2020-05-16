/*
README
ͼ��ƽ�����Ե��ȡ����
1.������Ŀǰֻ֧��Windowsϵͳ��ʹ����windows����̨����
2.������Ŀǰֻ֧��BMP�ļ���ʽ��ͼƬ���ɽ�������ʽͼƬͨ��ϵͳ�Դ���ͼ���ΪBMP��ʽ��
3.��������δ��������ͼƬ���ܣ������ͼƬǰ���뽫BMP��ʽͼƬ��������Ӧ�����ļ���·����
4.������ʹ�������ֲ��Ź��ܣ���׼�⺯���������������޷��������л������������֣��������������㣺
   (1) �����ļ�Ŀ¼���Ƿ��� Sounds\Home.wav�ļ�
   (2) ����undefined reference to `PlaySoundA@12'����project -> bulid options ��linker settings
         �ٵ�add ���winmm��ok���ɡ�����Ŀ-����ѡ��-����������-���ӿ����-����winmm-ok��
5.��л����BGM����ɾ��
   Home  ���� Toby Fox ��UNDERTALE Soundtrack��
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <time.h>
#include <mmsystem.h>//��������ͷ�ļ�
#include <io.h>     //��bug
#include <pthread.h>//��bug

#pragma comment(lib,"Winmm.lib")//������������

#define HEIGHT 40
#define WIDTH 118

int Running = 1;//���������б�־
HANDLE hOutput, hOutBuf;//����̨��Ļ���������
COORD coord = { 0,0 };
//˫���崦����ʾ��ʼ����
DWORD bytes = 0;

struct BMPFileName{
    char name[10][40];
    int count;
};
typedef struct BMPFileName BMPFile;
BMPFile BMP;//ͳ���ļ���BMPͼƬ�����ֺ�����

struct Pointer{
    int mode;// ֵΪ0��1,����ָ��������ұ�
    int PX[2],PY[2];// 0��1ģʽ��������ʼ��
    int PointerX,PointerY; //ָ������
    int PXE[2];// 0��1ģʽ��ָ�������յ�
};
struct Pointer Pointer;//����ָ��ṹ��

void InitPointer(){
    //��ʼ��ָ��
    Pointer.mode   = 0;
    Pointer.PX[0]  = 17;
    Pointer.PY[0]  = 29;
    Pointer.PX[1]  = 17;
    Pointer.PY[1]  = 59;
    Pointer.PXE[0] = 19+(BMP.count)*2;
    Pointer.PXE[1] = 35;
    Pointer.PointerX = Pointer.PX[0];
    Pointer.PointerY = Pointer.PY[0];
}

/**�����ȵ�ģ��**/
char news[5][80] = {
                    "�����������۶�4��,�ͷ�������������ü�",
                    "�����������ڵ���4��,�¶ẽ�յ�һ�˳ƺ�",
                    "���Ƶĳ�ȹ�����ɵ�ͷ��~",
                    "������N�ŷ����¼���������",
                    "���ҿ��ȱ���α��22�ڽ��׶�,��ǰ����80%"
                    };
/**��̬WELCOMEģ��**/
int WelcomeX = 0;
char Blank[200] = "                                                                                                                      ";
char Welcome[5][200] = {
        "BGM: Home                  **       *       **  ********   **         *******    *******     ****     ****   ******** ",
        "                            **     ***     **   **         **        **         *       *    ** **   ** **   **       ",
        "                             **   ** **   **    ********   **       **         *         *   **  ** **  **   ******** ",
        "                              ** **   ** **     **         **        **         *       *    **   ***   **   **       ",
        "                               ***     ***      ********   ********   *******    *******     **         **   ******** "
};
/**������ģ��**/
char data[HEIGHT][WIDTH] = {
        "",
        "",
        "",
        "",
        "",
        "                          ******************************************************************************************** ",
        "                          **                      **       **   ********   **    *   *     *                        ** ",
        "                          **                      * *     * *   **         * *   *   *     *                        ** ",
        "                          **                      *  *   *  *   ********   *  *  *   *     *                        ** ",
        "                          **                      *   * *   *   **         *   * *   *     *                        ** ",
        "                          **                      *    *    *   ********   *    **   *******                        ** ",
        "                          **----------------------------------------------------------------------------------------** ",
        "                          **                           ʹ�� ���� �����͡�Enter����ѡ�����                            **",
        "                          **----------------------------------------------------------------------------------------** ",
        "                          **   ����Ŀ¼�в��ҵ�   |                      ѡ���ͼƬ�Ĵ���ʽ                       ** ",
        "                          **       ����ͼƬ       |            (������ƽ���ٲ鿴��Եͼ���Ƽ���ֵƽ��)             ** ",
        "                          **----------------------|-----------------------------------------------------------------** ",
        "                          **                      |             �鿴ԭͼ                                            ** ",
        "                          **                      |                                                                 ** ",
        "                          **                      |             �鿴�Ҷ�ͼ                                          ** ",
        "                          **                      |                                                                 ** ",
        "                          **                      |             �鿴�ݶ�ͼ��                                        ** ",
        "                          **                      |                                                                 ** ",
        "                          **                      |             ��ֵ�˲�ƽ��                                        ** ",
        "                          **                      |                                                                 ** ",
        "                          **                      |             ��˹�˲�ƽ��                                        ** ",
        "                          **                      |                                                                 ** ",
        "                          **                      |             �Զ�����ƽ��                                      ** ",
        "                          **                      |                                                                 ** ",
        "                          **                      |             ��Ե������˹�˲�ƽ��                                ** ",
        "                          **                      |                                                                 ** ",
        "                          **                      |             ��ֵ�˲�ƽ������Чȥ������������                    ** ",
        "                          **                      |                                                                 ** ",
        "                          **                      |             �鿴��Եͼ��                                        ** ",
        "                          **                      |                                                                 ** ",
        "                          **                      |             ����ͼƬѡ��                                        ** ",
        "                          **----------------------------------------------------------------------------------------** ",
        "                          **                     �����ȵ㣺                                                         ** ",
        "                          ******************************************************************************************** "
        };

/**BMP�ļ����Һʹ�ӡ��������ģ��**/
void FindBMP(){
    //�ڹ���Ŀ¼\\pic��ͳ��BMP
    char *to_find = (char*)malloc(500*sizeof(char));
    _getcwd(to_find,500);
    strcat(to_find,"\\pic\\*.bmp");
    BMP.count = 0;
    long handle;
    struct _finddata_t fileinfo;
    handle = _findfirst(to_find,&fileinfo);
    strcpy(BMP.name[BMP.count++],fileinfo.name);
    while(!_findnext(handle,&fileinfo)){
        strcpy(BMP.name[BMP.count++],fileinfo.name);
    }
    _findclose(handle);
}

void SetBMP(){
    //��ӡBMP���Ƶ�������
    FindBMP();
    for(int i=0;i<BMP.count;i++){
        int length = strlen(BMP.name[i])-5;
        for(int j=0;j<18;j++){
            if(j<=length)
                data[i*2+17][j+32] = BMP.name[i][j];
            else
                data[i*2+17][j+32] = ' ';
        }
    }
    char s1[10] = "�����ĵ�";
    char s2[10] = "�˳�����";
    for(int j=0;j<18;j++){
        if(j<=8){
            data[BMP.count*2+17][j+32] = s1[j];
            data[BMP.count*2+19][j+32] = s2[j];
        }
        else{
            data[BMP.count*2+17][j+32] = ' ';
            data[BMP.count*2+19][j+32] = ' ';
        }
    }
}

/**��ʼ������**/
clock_t t1,t2;
void InitMenu(){
    //��ʼ��ҳ������
    if(WelcomeX>=0 && WelcomeX<=4)
        strcpy(data[WelcomeX],Welcome[WelcomeX]);
    if(WelcomeX>=10 && WelcomeX<=14){
        strcpy(data[14-WelcomeX],Blank);
    }
    WelcomeX++;
    if(WelcomeX>14)
        WelcomeX = 0;//��ʼ��welcome

    t2 = clock();
    int index = ((t2-t1)/3000)%5;
    int newslength = strlen(news[index]);
    for(int i=0;i<40;i++){
        if(i<=newslength)
            data[37][60+i] = news[index][i];
        else
            data[37][60+i] = ' ';//��ʼ�������ȵ�ģ��
    }
}

void SetPointer(){
    //����ָ��λ��
    data[Pointer.PointerX][Pointer.PointerY] = '-';
    data[Pointer.PointerX][Pointer.PointerY+1] = '-';
    data[Pointer.PointerX][Pointer.PointerY+2] = '>';
}


char help[22][66] = {
                        "                          �����ĵ�                           ",
                        "                    ������������ز˵���                     ",
                        "-----------------------------------------------------------------",
                        "",
                        "",
                        "",
                        "      README                                                 ",
                        "      ͼ��ƽ�����Ե��ȡ����                                 ",
                        "      1.������Ŀǰֻ֧��Windowsϵͳ��ʹ��                    ",
                        "                                                             ",
                        "      2.������Ŀǰֻ֧��BMP�ļ���ʽ��ͼƬ                    ",
                        "                                                             ",
                        "      3.��������δ��������ͼƬ���ܣ������ͼƬǰ���뽫BMP��ʽ",
                        "        ͼƬ�����ڹ���Ŀ¼��pic�ļ�����                      ",
                        "                                                             ",
                        "      4.��л����BGM����ɾ��                                  ",
                        "        Home  ���� Toby Fox ��UNDERTALE Soundtrack��           ",
                    };
char tempData[22][66];

void Help(){
    //��help�ĵ������ݴ�ӡ������
        for(int i=14;i<=35;i++){
            strncpy(data[i]+51,help[i-14],65);
        }
}

void SaveHelpData(){
    //��ʱ�������������tempData
    for(int i=14;i<=35;i++){
        strncpy(tempData[i-14],data[i]+51,65);
    }
}

void show(){
    //��Ҫ�Ľ�����ʾ����
    InitMenu();//��ʼ������
    SetPointer();//����ָ��λ��
    /*˫�������������������*/
    for (int i=0;i<HEIGHT;i++){
        coord.Y = i;
        WriteConsoleOutputCharacterA(hOutBuf, data[i], WIDTH, coord, &bytes);
    }
    //�����µĻ�����Ϊ���ʾ����
    SetConsoleActiveScreenBuffer(hOutBuf);
    Sleep(60);

    for (int i=0;i<HEIGHT;i++){
        coord.Y = i;
        WriteConsoleOutputCharacterA(hOutput, data[i], WIDTH, coord, &bytes);
    }
    //�����µĻ�����Ϊ���ʾ����
    SetConsoleActiveScreenBuffer(hOutput);
    Sleep(60);
}

/**ͼ����ģ��**/
int W,H;//ͼ��Ŀ�͸�
int HighLimit=100,LowLimit=50;//Ĭ��ֵ�� RMS��50
char **img;//ͼ�����ݴ�������
char **tempImg;//��ʱͼ�����ݴ�������
int flag = 0;//�Ƿ���Ҫ��ʾͼ��
char resFilename[10] = "res0.bmp";//���ɴ����ͼ�������

/**���ĺ����б�**/
char** ReadBMP(char* filename);
    //��ȡBMPͼ�񣬷��ش���ͼ�����ݵĶ�άchar����
void WriteBMP(char**img,const char* filename);
    //����BMPͼƬ���������24λ���ͼ�򵥴��������ýṹ��������ݡ�
char** Copy(char**src);
    //����һ����src����һ���Ķ�ά����
int change(char n);
    //��ͼ��С��0�����ݽ��д���
char** GrayImg(char**img);
    //��ȡimg�ĻҶ�ͼ�����ص�ͨ���Ҷ�ͼ������
int** TiImg(char** Gray);
    //����Sobel���Ӽ�����Ҷ�ͼ���ݶ�ͼ
void GrayLv(char**img);
    //��imgת��Ϊ�Ҷ�ͼ
void TiLv(char** img);
    //��imgת�����ݶ�ͼ��
void AverageLv(char**img);
    //��ֵ�˲�
void GaosiLv(char **img);
    //��˹����
void idyLv(char**img, int Juanji[3][3]);
    //�Զ������˲� 3*3���ģ��
void EdgeGaosiLv(char**img);
    //��˹�˲� 5*5���ģ��
void MidValueLv(char**img);
    //��ֵ����
int Round(int**Ti,int i,int j);
    //Ѱ���ݶ�ͼ���а˸���λ�Ƿ����ݶȴ��ڸ���ֵ�ĵ�
void Canny(char**img,int N);
    //���ڸ���ֵ�ĵ���Ϊ��Ե�����ڵ���ֵ���ڸ���ֵ�ĵ㣬��Round����ֵΪ1����Ϊ��Ե��
int RMS(char**img);
    //����img�ݶ�ͼ�����ݵľ�����
void DFS(int i,int j, int EI);
    //��������㷨
void DFSImg(char**img);
    //�������ͼ��ı�Ե

/**����ʵ��**/
char** ReadBMP(char* filename){
    //��ȡBMPͼ�񣬷��ش���ͼ�����ݵĶ�άchar����

    FILE *fp=fopen(filename,"rb");//�����ƶ���ʽ��ָ����ͼ���ļ�
    int a[200];//��BMPͼ��Ĳ�������
    char t[2]; //��BMPͼ���ַ���ͷ"BM"
    fread(t,2,1,fp);
    fread(a,52,1,fp);
    W = a[4]; // ��
    H = a[5]; // ��
    //��ȡͼ�������Ϣ

    //�������ͼ���������ڴ�����Ĵ洢
    char** ImageData = (char**)malloc(H*sizeof(char*));
    for (int i=0;i<H;i++){
        ImageData[i]=(char*)malloc((W*3+3)/4*4*sizeof(char));
    }
    for (int k=0;k<H;k++ ){
        for(int j=0;j<(W*3+3)/4*4;j++){
            fread(&ImageData[k][j],1,1,fp);//������ɶ�̬��ά��������룬����ʵ�ʶ�ȡͼ������
        }
    }
    fclose(fp);//�ر��ļ�
    return ImageData;
}

void WriteBMP(char**img,const char* filename)
{   //����BMPͼƬ���������24λ���ͼ�򵥴��������ýṹ��������ݡ�
    int l=(W*3+3)/4*4;
    int bmi[]= {l*H+54,0,54,40,W,H,1|3*8<<16,0,l*H,0,0,100,0};//����BMP��ʽ��Ϣ����
    FILE *fp = fopen(filename,"wb");
    fprintf(fp,"BM");
    fwrite(&bmi,52,1,fp);
    for(int i=0;i<H;i++){
        fwrite(img[i],1,l,fp);
    }
    fclose(fp);
}

char** Copy(char**src){
    //����һ����src����һ���Ķ�ά����
    int l = (W*3+3)/4*4;
    char**tar;
    tar = (char**)malloc(H*sizeof(char*));
    for (int i=0;i<H;i++){
        tar[i]=(char*)malloc(l*sizeof(char));
    }
    for(int i=0;i<H;i++)
        for(int j=0;j<l;j++)
            tar[i][j] = src[i][j];
    return tar;
}

int change(char n){
    //��ͼ��С��0�����ݽ��д���
    if(n<0) return 256+n;
    else return n;
}

char** GrayImg(char**img){
    //��ȡimg�ĻҶ�ͼ�����ص�ͨ���Ҷ�ͼ������
    char** Gray = (char**)malloc(H*sizeof(char*));
    for(int i=0;i<H;i++){
        Gray[i] = (char*)malloc(W*sizeof(char));
    }
    /* G = 0.3*B+0.59*G+0.11*R */
    for(int i=0;i<H;i++){
        for(int j=0;j<W;j++){
            Gray[i][j] = (change(img[i][j*3])*30 + change(img[i][j*3+1])*59 + change(img[i][j*3+2])*11)/100;
            if(Gray[i][j]>=128){
                Gray[i][j] -= 256;
            }
        }
    }
    return Gray;
}

int** TiImg(char** Gray){
    //����Sobel���Ӽ�����Ҷ�ͼ���ݶ�ͼ
    int** Ti = (int**)malloc(H*sizeof(int*));
    for(int i=0;i<H;i++){
        Ti[i] = (int*)malloc(W*sizeof(int));
    }
    for(int j=0;j<W;j++){
        Ti[0][j] = 0;
        Ti[H-1][j] = 0;
    }
    for(int i=0;i<H;i++){
        Ti[i][0] = 0;
        Ti[i][W-1] = 0;
    }
    int JuanjiX[3][3] = { {-1,0,1},{-2,0,2},{-1,0,1} };
    int JuanjiY[3][3] = { {-1,-2,-1},{0,0,0},{1,2,1} };//Sobel����������
    for(int i=1;i<H-1;i++){
        for(int j=1;j<W-1;j++){
            int Gx = 0;
            for(int x=i-1;x<=i+1;x++)
                for(int y=j-1;y<=j+1;y++)
                    Gx += change(Gray[x][y])*JuanjiX[x-i+1][y-j+1];
            int Gy = 0;
            for(int x=i-1;x<=i+1;x++)
                for(int y=j-1;y<=j+1;y++)
                    Gy += change(Gray[x][y])*JuanjiY[x-i+1][y-j+1];

            Ti[i][j] = (int)sqrt(Gx*Gx+Gy*Gy);
            if(Ti[i][j]>255) Ti[i][j] = 255;
        }
    }
    return Ti;
}

void GrayLv(char**img){
    //��imgת��Ϊ�Ҷ�ͼ
    char** Gray = GrayImg(img);
    for(int i=1;i<H-1;i++){
        for(int j=1;j<W-1;j++){
            img[i][j*3] = img[i][j*3+1] = img[i][j*3+2] = Gray[i][j];
        }
    }
}

void TiLv(char** img){
    //��imgת�����ݶ�ͼ��
    char**Gray = GrayImg(img);
    int** Ti = TiImg(Gray);
    for(int i=1;i<H-1;i++){
        for(int j=1;j<W-1;j++){
            img[i][j*3] = img[i][j*3+1] = img[i][j*3+2] = Ti[i][j];
        }
    }
}

void AverageLv(char**img){
    //��ֵ�˲�
    int** Ti = TiImg(GrayImg(img));
    for(int i=2;i<H-2;i++){
        for(int j=2;j<W-2;j++){
            for(int k=0;k<3;k++){
                int sum = 0;
                for(int x=i-2;x<=i+2;x++)
                    for(int y=j-2;y<=j+2;y++){
                        int value = img[x][y*3+k];
                        if(value<0){
                            value += 256;
                        }
                        sum += value;
                    }
                int res = sum/25;
                if(res>=255){
                    res = 255;
                }
                img[i][j*3+k] = res;
            }
        }
    }
}

void GaosiLv(char **img){
    //��˹����
    int** Ti = TiImg(GrayImg(img));
    int Juanji[5][5] = {
                        {1,4,7,4,1},
                        {4,16,26,16,4},
                        {7,26,41,26,7},
                        {4,16,26,16,4},
                        {1,4,7,4,1}
                    };//��˹�����
    for(int i=2;i<H-2;i++){
        for(int j=2;j<W-2;j++){
            for(int k=0;k<3;k++){
                int sum = 0;
                for(int x=i-2;x<=i+2;x++)
                    for(int y=j-2;y<=j+2;y++){
                        int value = img[x][y*3+k];
                        if(value < 0){
                            value += 256;
                        }
                        sum += value*Juanji[x-i+2][y-j+2];
                    }
                int res = sum/273;
                if(res>=128){
                    res -= 256;
                }
                img[i][j*3+k] = res;
            }
        }
    }
}

void idyLv(char**img, int Juanji[3][3]){
    //�Զ������˲� 3*3���ģ��
    int l = (W*3+3)/4*4;
    int** Ti = TiImg(GrayImg(img));
    int JuanjiSum = 0;
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            JuanjiSum += Juanji[i][j];
    for(int i=1;i<H-1;i++){
        for(int j=1;j<W-1;j++){
            if(Ti[i][j]<HighLimit)
                for(int k=0;k<3;k++){
                    int sum = 0;
                    for(int x=i-1;x<=i+1;x++)
                        for(int y=j-1;y<=j+1;y++){
                            int value = img[x][y*3+k];
                            if(value < 0){
                                value += 256;
                            }
                            sum += value*Juanji[x-i+1][y-j+1];
                        }
                    int res = sum/JuanjiSum;
                    if(res>=128){
                        res -= 256;
                    }
                    img[i][j*3+k] = res;
                }
        }
    }
}

void EdgeGaosiLv(char**img){
    //��˹�˲� 5*5���ģ��
    int** Ti = TiImg(GrayImg(img));
    int Juanji[5][5] = {
                        {1,4,7,4,1},
                        {4,16,26,16,4},
                        {7,26,41,26,7},
                        {4,16,26,16,4},
                        {1,4,7,4,1}
                    };
    for(int i=2;i<H-2;i++){
        for(int j=2;j<W-2;j++){
            if(Ti[i][j]<HighLimit)//����ݶ�ֵ���ڸ���ֵ�������и�˹ģ��
                for(int k=0;k<3;k++){
                    int sum = 0;
                    for(int x=i-2;x<=i+2;x++)
                        for(int y=j-2;y<=j+2;y++){
                            int value = img[x][y*3+k];
                            if(value < 0){
                                value += 256;
                            }
                            sum += value*Juanji[x-i+2][y-j+2];
                        }
                    int res = sum/273;
                    if(res>=128){
                        res -= 256;
                    }
                    img[i][j*3+k] = res;
                }
        }
    }
}

int cmp(const void*a, const void*b){
    //��������ȽϺ���
    return *(int*)a - *(int*)b;
}

void MidValueLv(char**img){
    //��ֵ����
    int l = (W*3+3)/4*4;
    for(int i=1;i<H-1;i++){
        for(int j=3;j+3<l;j++){
            int count = 0;
            int value[9] = {0};
            for(int x=i-1;x<=i+1;x++)
                for(int y=j-3;y<=j+3;y+=3)
                    value[count++] = change(img[x][y]);
            qsort(value,9,sizeof(int),cmp);
            if(value[4]>=128){
                value[4] -= 256;
            }
            img[i][j] = value[4];
        }
    }
}

int Round(int**Ti,int i,int j){
    //Ѱ���ݶ�ͼ���а˸���λ�Ƿ����ݶȴ��ڸ���ֵ�ĵ�
    for(int x=i-1;x<=i+1;x++)
        for(int y=j-1;y<=j+1;y++)
            if(Ti[x][y] >= HighLimit)
                return 1;
    return 0;
}

void Canny(char**img, int N){
    //���ڸ���ֵ�ĵ���Ϊ��Ե�㣬���ڵ���ֵ���ڸ���ֵ�ĵ㣬��Round����ֵΪ1����Ϊ��Ե��
    //����N(1-9)��ֵ�ı�HighLimit��ֵ,�Ӷ��ı��Ե��;1-5-9:��-����-��
    int l = (W*3+3)/4*4;
    char** Gray = GrayImg(img);
    HighLimit += (5-N)*10;
    int** Ti = TiImg(Gray);
    for(int i=1;i<H-1;i++){
        for(int j=1;j<W-1;j++){
            if(Ti[i][j]>=HighLimit)
                img[i][j*3] = img[i][j*3+1] = img[i][j*3+2] = Ti[i][j];
            else if(Ti[i][j]>=LowLimit && Round(Ti,i,j))
                img[i][j*3] = img[i][j*3+1] = img[i][j*3+2] = Ti[i][j];
            else
                img[i][j*3] = img[i][j*3+1] = img[i][j*3+2] = 0;
        }
    }
}

int RMS(char**img){
    //��ͼ�����ݵľ�����RMS
    int **Ti = TiImg(GrayImg(img));
    long long sum = 0;
    for(int i=1;i<H-1;i++)
        for(int j=1;j<W-1;j++)
            sum += Ti[i][j]*Ti[i][j];
    long long RMS = sqrt(sum/((W-2)*(H-2)));
    return RMS;
}

int** visited;      //�ݶ�ͼ��ĵ�ͼ��
int EdgeLength[5000];   //��Ե����
int EdgeIndex = 0;      //��Ե�����±�
void DFS(int i,int j,int EI){
    //��������㷨
    if(visited[i][j] == 1) //�ѷ��ʹ�
            return;
    visited[i][j] = 1;
    EdgeLength[EI]++;
    //����ͨ����
    if(i-1>=1){
        if(j-1>=1 && visited[i-1][j-1]==0)
            DFS(i-1,j-1,EI);//����

        if(j+1<=W-1 && visited[i-1][j+1]==0)
            DFS(i-1,j+1,EI);//����

        if(visited[i-1][j]==0)
            DFS(i-1,j,EI);//����
    }
    if(i+1<=H-1){
        if(j-1>=1 && visited[i+1][j-1]==0)
            DFS(i+1,j-1,EI);//����

        if(j+1<=W-1 && visited[i+1][j+1]==0)
            DFS(i+1,j+1,EI);//����

        if(visited[i+1][j]==0)
            DFS(i+1,j,EI);//����
    }
    if(j-1>=1 && visited[i][j-1]==0)
            DFS(i,j-1,EI);//��

    if(j+1<=W-1 && visited[i][j+1]==0)
            DFS(i,j+1,EI);//��
}

void DFSImg(char**img){
    //�������ͼ��ı�Ե
    memset(EdgeLength,0,sizeof(EdgeLength));
    visited = (int**)malloc(H*sizeof(int*));
    for(int i=0;i<H;i++)
        visited[i] = (int*)malloc(W*sizeof(int));//Ϊvisited���������ڴ�ռ�
    int** Ti = TiImg(GrayImg(img));
    for(int i=1;i<H-1;i++){
        for(int j=1;j<W-1;j++)
            if(Ti[i][j]>=HighLimit || (Ti[i][j]>=LowLimit && Round(Ti,i,j)))
                visited[i][j] = 0;//�ɷ���
            else
                visited[i][j] = -1;//���ɷ���
    }
    for(int i=1;i<H-1;i++)
        for(int j=1;j<W-1;j++)
            if(visited[i][j]==0){
                DFS(i,j,EdgeIndex);
                EdgeIndex++;
            }
}

//�����������
int JuanjiFlag = 0;//�Ƿ��Զ�������
int JuanCount = 0; //����˱�����
int EdgeFlag = 0;  //�Ƿ�鿴��Ե
int idyJuan[3][3]; //�Զ�������
int HelpFlag = 0;  //�Ƿ�򿪰����ĵ�
void Moving(){
    if(_kbhit()){
        //������������

        if(HelpFlag == 1){
            //�򿪰����ĵ�
            _getch();
            for(int i=14;i<=35;i++)
                strncpy(data[i]+51,tempData[i-14],65);
            HelpFlag = 0;
        }

  else {
            //�򿪰����ĵ����ѡ��
        char temp = _getch();
        switch(temp){
            case 72 :   data[Pointer.PointerX][Pointer.PointerY] = data[Pointer.PointerX][Pointer.PointerY+1] = data[Pointer.PointerX][Pointer.PointerY+2] = ' ';
                        if(Pointer.PointerX-2 >= Pointer.PX[Pointer.mode])
                            Pointer.PointerX -= 2;
                        else
                            Pointer.PointerX = Pointer.PXE[Pointer.mode];
                        break;
                        //���������

            case 80 :   data[Pointer.PointerX][Pointer.PointerY] = data[Pointer.PointerX][Pointer.PointerY+1] = data[Pointer.PointerX][Pointer.PointerY+2] = ' ';
                        if(Pointer.PointerX+2 <= Pointer.PXE[Pointer.mode])
                            Pointer.PointerX += 2;
                        else
                            Pointer.PointerX = Pointer.PX[Pointer.mode];
                        break;
                        //���������

            case 13:    data[Pointer.PointerX][Pointer.PointerY] = data[Pointer.PointerX][Pointer.PointerY+1] = data[Pointer.PointerX][Pointer.PointerY+2] = ' ';
                        if(Pointer.mode==0) {
                            //ָ�������ʱ
                            if(Pointer.PointerX == Pointer.PXE[0]){
                                Running = 0;
                            }
                            else if(Pointer.PointerX == Pointer.PXE[0]-2){
                                SaveHelpData();
                                Help();
                                HelpFlag = 1;
                            }
                            else{
                                char srcFilename[30] = "pic\\";
                                strcat(srcFilename,BMP.name[(Pointer.PointerX-Pointer.PX[0])/2]);
                                img = ReadBMP(srcFilename);
                                tempImg = Copy(img);
                                Pointer.mode = 1;
                                Pointer.PointerX = Pointer.PX[Pointer.mode];
                                Pointer.PointerY = Pointer.PY[Pointer.mode];
                            }
                        }
                        else {
                            //�������ұ�ʱ
                            if(Pointer.PointerX == Pointer.PXE[1]){
                                Pointer.mode = 0;
                                Pointer.PointerX = Pointer.PX[Pointer.mode];
                                Pointer.PointerY = Pointer.PY[Pointer.mode];
                            }
                            else {
                                //�����ļ�������
                                resFilename[3]++;
                                if(resFilename[3]-'0'>=10)
                                    resFilename[3] = '0';

                                switch( (Pointer.PointerX-Pointer.PX[1])/2 ){
                                    case 0: WriteBMP(img,resFilename);
                                            flag = 1;
                                            break;
                                    case 1: tempImg = Copy(img);
                                            GrayLv(tempImg);
                                            WriteBMP(tempImg,resFilename);
                                            flag = 1;
                                            break;
                                    case 2: tempImg = Copy(img);
                                            TiLv(tempImg);
                                            WriteBMP(tempImg,resFilename);
                                            flag = 1;
                                            break;
                                    case 3: tempImg = Copy(img);
                                            AverageLv(tempImg);
                                            WriteBMP(tempImg,resFilename);
                                            flag = 1;
                                            break;
                                    case 4: tempImg = Copy(img);
                                            GaosiLv(tempImg);
                                            WriteBMP(tempImg,resFilename);
                                            flag = 1;
                                            break;
                                    case 5: tempImg = Copy(img);
                                            JuanjiFlag = 1;
                                            char Juanji[8][30] = {
                                                "������3*3(0-9)��������",
                                                "----------------------",
                                                "|      |      |      |",
                                                "----------------------",
                                                "|      |      |      |",
                                                "----------------------",
                                                "|      |      |      |",
                                                "----------------------",
                                            };
                                            for(int i=0;i<8;i++)
                                                for(int j=0;j<22;j++)
                                                    data[i+17][88+j] = Juanji[i][j];
                                            break;
                                    case 6: tempImg = Copy(img);
                                            EdgeGaosiLv(tempImg);
                                            WriteBMP(tempImg,resFilename);
                                            flag = 1;
                                            break;
                                    case 7: tempImg = Copy(img);
                                            MidValueLv(tempImg);
                                            WriteBMP(tempImg,resFilename);
                                            flag = 1;
                                            break;
                                    case 8: EdgeFlag = 1;
                                            char Edge[8][28] = {
                                                " ��Ե������0-5-9:��-����-��",
                                                "----------------------------",
                                                "|�������Ե������N:        |",
                                                "----------------------------",
                                                "|ͼ���ܱ�Ե����Ϊ:         |",
                                                "|��ı�Ե����Ϊ:         |",
                                                "|��̵ı�Ե����Ϊ:         |",
                                                "----------------------------",
                                            };
                                            for(int i=0;i<8;i++)
                                                for(int j=0;j<28;j++)
                                                    data[i+17][82+j] = Edge[i][j];
                                            break;
                                    default: break;
                                }
                            }
                        }
                        break;
                        //�������롮Enter��
        }

        //�Զ���������
        if(JuanjiFlag == 2 || (JuanjiFlag==1 && (temp==72||temp==80))){
            //���������ȡ���Զ�������Ϣ��ʾ
            for(int i=0;i<8;i++)
                for(int j=0;j<22;j++)
                    data[i+17][88+j] = ' ';
            JuanjiFlag = 0;
            JuanCount = 0;
        }
        if( JuanjiFlag==1 && temp>='0'&&temp<='9'){
            int Juan[9];
            data[(JuanCount/3)*2+19][(JuanCount)%3*7+91] = temp;
            idyJuan[JuanCount/3][JuanCount%3] = temp-'0';
            JuanCount++;
            if(JuanCount==9) {
                JuanCount = 0;
                JuanjiFlag = 2;
                idyLv(tempImg,idyJuan);
                WriteBMP(tempImg,resFilename);
                flag = 1;
            }
        }

        //�鿴��Ե����
        if(EdgeFlag == 2 || (EdgeFlag==1 && (temp==72||temp==80))){
            //���������ȡ����Ե��Ϣ��ʾ
            for(int i=0;i<8;i++)
                for(int j=0;j<28;j++)
                    data[i+17][82+j] = ' ';
            EdgeFlag = 0;
        }
        if( EdgeFlag==1 && temp>='0'&&temp<='9'){
            int N = 5;//Ĭ�ϱ�Ե������Ϊ5

            data[19][106] = temp; //��N��ʾ������
            N = temp-'0';
            DFSImg(tempImg);        //��ͼ���������
            int num = EdgeIndex;
            qsort(EdgeLength,EdgeIndex,sizeof(int),cmp);//�Ա�Ե���Ƚ�������
            int MinL = EdgeLength[0];
            int MaxL = EdgeLength[EdgeIndex-1];
            for(int i=0;i<6;i++){
                if(num){
                    data[21][106-i] = (num%10)+'0';
                    num /= 10;
                }
                if(MaxL){
                    data[22][106-i] = (MaxL%10)+'0';
                    MaxL /= 10;
                }
                if(MinL){
                    data[23][106-i] = (MinL%10)+'0';
                    MinL /= 10;
                }
            }
            EdgeIndex = 0;
            Canny(tempImg,N);
            WriteBMP(tempImg,resFilename);
            flag = 1;
            EdgeFlag = 2;
        }
    }
    }
}

int Emoji = 1;
int direction = 1;
void BYE(){
    //�˳�����
    int height = 8;
    char bye[8][200] = {
        "  *****    *    *   ******      *****    *    *   ******     ",
        "  **   *    *  *    **          **   *    *  *    **         ",
        "  *****      **     ******      *****      **     ******     ",
        "  **   *     **     **          **   *     **     **         ",
        "  *****      **     ******      *****      **     ******     ",
        "                        ��������˳�                         ",
        "                                                             "
    };
    /*�Ա���ͷ���в���*/
    Emoji += direction;
    strncpy(bye[height-1]+Emoji,"\\(^.^)/",7);
    if(Emoji == 50){
        direction = -1;
    }
    if(Emoji == 1){
        direction = 1;
    }

    /*˫�������������������*/
    COORD coord = {40,0};
    for (int i=0;i<height;i++){
        coord.Y = i+15;
        WriteConsoleOutputCharacterA(hOutBuf, bye[i], WIDTH, coord, &bytes);
    }
    //�����µĻ�����Ϊ���ʾ����
    SetConsoleActiveScreenBuffer(hOutBuf);
    Sleep(60);

    if(Emoji%5==0){
        //����������˸
        strncpy(bye[5]+23,"            ",13);
    }
    for (int i=0;i<height;i++){
        coord.Y = i+15;
        WriteConsoleOutputCharacterA(hOutput, bye[i], WIDTH, coord, &bytes);
    }
    //�����µĻ�����Ϊ���ʾ����
    SetConsoleActiveScreenBuffer(hOutput);
    Sleep(60);
}

void OpenHandle(){
    //��ȡ����̨��Ļ���������
    hOutBuf = CreateConsoleScreenBuffer(
        GENERIC_WRITE,//������̿�����������д����
        FILE_SHARE_WRITE,//���建�����ɹ���дȨ��
        NULL,
        CONSOLE_TEXTMODE_BUFFER,
        NULL
    );
    hOutput = CreateConsoleScreenBuffer(
        GENERIC_WRITE,//������̿�����������д����
        FILE_SHARE_WRITE,//���建�����ɹ���дȨ��
        NULL,
        CONSOLE_TEXTMODE_BUFFER,
        NULL
    );
}

void HideCursor(){
    //���ؿ���̨���
    CONSOLE_CURSOR_INFO cci;
    cci.bVisible = 0;
    cci.dwSize = 1;
    SetConsoleCursorInfo(hOutput, &cci);
    SetConsoleCursorInfo(hOutBuf, &cci);
}

void InitAll(){
    //PlaySound("Sounds\\Home.wav",NULL,SND_FILENAME | SND_ASYNC | SND_LOOP);
    //�������֣����������Ķ����뿪ͷ �� README.txt ��ע�͵����д���
    t1 = clock();
    //��¼����ʼʱ��
    system("mode con cols=140 lines=42");
    //���ÿ���̨���ڴ�С
    SetConsoleTitle("ͼ����");
    //���ÿ���̨����
    system("color 0A");
    //���ÿ���̨����ͱ�����ɫ
    OpenHandle();
    //�����µĿ���̨������
    HideCursor();
    //���������������Ĺ��
    SetBMP();
    //���Ҳ�����ɲ���BMP�ļ���
    InitPointer();
    //��ʼ��ָ��
}

int main(){
    InitAll();
    //��ʼ���������ã�BGM,����̨,BMP���ҵȣ�

    while(Running){
        //����������
        show();
        Moving();
        if(flag){
            //��ѡ����ʾͼƬʱ
            CloseHandle(hOutBuf);
            CloseHandle(hOutput);
            //�ر��������������
            system(resFilename);
            OpenHandle();
            //���´򿪾��
            HideCursor();
            flag = 0;
        }
    }

    //�˳�����
    CloseHandle(hOutBuf);
    CloseHandle(hOutput);
    OpenHandle();
    HideCursor();
    while(!_kbhit()){
        //��������˳�����
        BYE();
    }
    return 0;
}
