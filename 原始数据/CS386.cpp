#include<iostream>
#include<opencv2/opencv.hpp>
#include<vector>
#include<string>
#include<cmath>
 
using namespace cv;
using namespace std;

const int n = 4, m = 3;
double area[30] = {}, q_Mat[10][10] = {};

//输入训练照片集
void image_In()
{
	string str = ".jpg", str1 = "_1.jpg", str2 = "_2.jpg", str3 = "_3.jpg";
	for(char k = 'a'; k <= 't'; ++k)
	{
		//读入图像
		Mat srcImage = imread(k + str);
		//首先对图像进行空间的转换
		Mat grayImage;
		Mat bijImage;
		cvtColor(srcImage, grayImage, CV_BGR2GRAY);		
		imwrite(k + str1, grayImage);
		//对灰度图进行滤波
		GaussianBlur(grayImage, grayImage, Size(3, 3), 0, 0);
		imwrite(k + str2, grayImage);
		//为了得到二值图像，对灰度图进行边缘检测
		Mat cannyImage;
		Canny(grayImage, cannyImage, 70, 50, 3);
		imwrite(k + str3, cannyImage);
		//在得到的二值图像中寻找轮廓
		vector< vector<Point> > contours;
		vector< Vec4i > hierarchy;
		findContours(cannyImage, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
		//绘制轮廓
		for (int i = 0; i < (int)contours.size(); ++i)
		{
			drawContours(cannyImage, contours, i, Scalar(255), 1, 8);
		}
		//计算轮廓的面积
		for (int i = 0; i < (int)contours.size(); ++i)
		{
			double g_dConArea = contourArea(contours[i], false);
			area[k - 'a'] += g_dConArea;
		}
	}
}

//利用快速幂求x^y
double my_Power(double x, int y)
{
    double tmp = x, ans = 1;
    while(y)
    {
        if(y & 1)
        {
            ans *= tmp;
        }
        y >>= 1;
        tmp *= tmp;
    }
    return ans;
}

int my_Max(int x, int y)
{
    return (x > y) ? x : y;
}

//回归矩阵输出函数
void Out()
{
    for(int i = 0; i <= m; ++i)
    {
        for(int j = 0; j <= m + 1; ++j)
        {
            cout << q_Mat[i][j] << " ";
        }
        cout << endl;
    }
    cout << "-----------------\n";
}

//初始化回归矩阵
void init_Mat()
{
    for(char k = 'a'; k <= 't'; ++k)
    {
        int x, y;
        double s, v, tmp;
		s = area[k - 'a'];
        cin >> v; //输入体积
		//v = (k - 'a') * s;
        s = sqrt(sqrt(s));
		//cout << s << " ";
		cout << "The area of No." << k << " picture is " << s * s << "\t\t"; //输入测试数据面积
		cout << "The volume of No." << k << " picture is " << v << endl; //输入测试数据体积
		//计算回归矩阵
        for(int j = 0; j <= 2 * m; ++j)
        {
            tmp = my_Power(s, j);
            x = my_Max(0, j - m);
            y = j - x;
            while(x <= m && y >= 0)
            {
                q_Mat[x++][y--] += tmp;
            }
        }
        for(int j = 0; j <= m; ++j)
        {
            tmp = my_Power(s, j) * v;
            q_Mat[j][m + 1] += tmp;
        }
    }
    //Out();
}

//利用回归矩阵计算函数参数
void calc_Mat()
{
	//利用矩阵行变换进行求解方程组
    for(int i = 0; i <= m; ++i)
    {
        for(int j = 0;j < i; ++j)
        {
            for(int k = j + 1; k <= m + 1; ++k)
            {
                q_Mat[i][k] -= q_Mat[i][j] * q_Mat[j][k];
            }
            q_Mat[i][j] = 0;
        }
        for(int j = i + 1; j <= m + 1; ++j)
        {
            q_Mat[i][j] /= q_Mat[i][i];
        }
        q_Mat[i][i] = 1;
        //Out();
    }
    for(int i = m; i >= 0; --i)
    {
        for(int j = i + 1; j <= m; ++j)
        {
            q_Mat[i][m + 1] -= q_Mat[i][j] * q_Mat[j][m + 1];
            q_Mat[i][j] = 0;
        }
    }
	//解为(q_Mat[0][m + 1], q_Mat[1][m + 1], ..., q_Mat[m][m + 1])
	//Out();
}

//测试照片输入
void data_In()
{
	string str = ".jpg", str1 = "_1.jpg", str2 = "_2.jpg", str3 = "_3.jpg";
	for(char k = '1'; k <= n + '0'; ++k)
	{
		//读入图像
		Mat srcImage = imread(k + str);
		//首先对图像进行空间的转换
		Mat grayImage;
		Mat bijImage;
		cvtColor(srcImage, grayImage, CV_BGR2GRAY);
		imwrite(k + str1, grayImage);
		//对灰度图进行滤波
		GaussianBlur(grayImage, grayImage, Size(3, 3), 0, 0);
		imwrite(k + str2, grayImage);
		//为了得到二值图像，对灰度图进行边缘检测
		Mat cannyImage;
		Canny(grayImage, cannyImage, 255, 128, 3);
		imwrite(k + str3, cannyImage);
		//在得到的二值图像中寻找轮廓
		vector< vector<Point> > contours;
		vector< Vec4i > hierarchy;
		findContours(cannyImage, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
		//绘制轮廓
		for (int i = 0; i < (int)contours.size(); ++i)
		{
			drawContours(cannyImage, contours, i, Scalar(255), 1, 8);
		}
		//计算轮廓的面积
		for (int i = 0; i < (int)contours.size(); ++i)
		{
			double g_dConArea = contourArea(contours[i], false);
			area[k - '0'] += g_dConArea;
		}
	}
}

//计算测试照片体积
void calc_Volume()
{
	double s, v;
	for(int i = 1; i <= n; ++i)
	{
		s = sqrt(sqrt(area[i])); //取面积的二分之一次方
		v = 0;
		for(int j = 0; j <= m; ++j)
		{
			v += q_Mat[j][m + 1] * my_Power(s, j); //根据函数，输入面积计算体积
		}
		cout << "The area of No." << i << " picture is " << s * s << "\t\t"; //输入测试数据面积
		cout << "The volume of No." << i << " picture is " << v << endl; //输入测试数据体积
	}
}

int main()
{
	freopen("data.txt", "r", stdin);
	freopen("result.txt", "w", stdout);
	//输入训练照片集
	image_In();
	//初始化回归矩阵
    init_Mat();
	//利用回归矩阵计算函数参数
    calc_Mat();
	//测试照片输入
	data_In();
	//计算测试照片体积
	calc_Volume();
	cout << "The function is : V = " << q_Mat[0][m + 1];
	for(int i = 1; i <= m; ++i)
	{
		if(q_Mat[i][m + 1] >= 0) cout << " + ";
		else cout << " - ";
		if(q_Mat[i][m + 1] != 0) 
		{		
			cout << (q_Mat[i][m + 1] >= 0 ? q_Mat[i][m + 1] : -q_Mat[i][m + 1]);
			cout << " S ^ ( " << i << " / 2 )";
		}
	}
	fclose(stdin);
	fclose(stdout);
	//waitKey(0);
 
	return 0;
}

