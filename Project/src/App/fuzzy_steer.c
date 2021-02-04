#include "fuzzy_steer.h"

#define NB 0
#define NM 1
#define NS 2
#define ZO 3
#define PS 4
#define PM 5
#define PB 6
float E_Value;                   //偏差的量化级别
float EC_Value;                  //偏差的量化级别
float e_Max = 15.0;                     //偏差范围
float e_Min = -15.0;
float ec_Max = 3.0;                    //偏差变化率范围
float ec_Min = -3.0; 
float DirP_Value = 0.0;                         //输出值--P的量化级别 
struct Fuzzy_Membership          //隶属度结构体
{
  int Set_1;
  int Set_2;
  int Set_3;
  int Set_4;
  float Set_1_Member;
  float Set_2_Member;
  float Set_3_Member;
  float Set_4_Member;
};
struct Fuzzy_Membership E_Member;
struct Fuzzy_Membership EC_Member;
struct Fuzzy_Membership DirP_Member;
struct Fuzzy_Membership DirD_Member;


int Fuzzy_DirP_Rule[7][7] =      //方向P模糊规则表 19-7-13 8:51，调整4，原因：减小抖动
{
    {6, 6, 6, 6, 6, 4, 6},
    {-4, -2, -2, 6, 4, 4, 6},
    {6, 4, 6, 2, 2,6,6},
    {4, 4, 2, 6, 6,-2,-2},
    {6, 6, 2, 6, 6, 4, 6},
    {6, 4, 4, 6, -2, -2, -4},
    {6, 4, 6, 6, 6, 6, 6}
};

int Fuzzy_DirD_Rule[7][7] =      //方向D模糊规则表
{
    {6,  4,  2, -2, 0, -2, -2},
    {6,  4,  2,  0,-2, -4, -6},
    {6,  4,  2,  0, 0, -2,  2},
    {2, -2, -2, -2,-2, -3,  2},
    {2, -2,  0,  0, 2,  4,  6},
    {-6,-4, -2,  0, 2,  4,  6},
    {-2,-2,  0, -2, 2,  4,  6}
};


/*
* 自适应模糊PID--P
* 动态调整P的大小
*/
float Fuzzy_DirP(float e, float ec)
{
  LinearQuantization(e, ec);                            //计算量化级别
  E_Membership(E_Value);                                 //计算偏差的隶属度
  EC_Membership(EC_Value);                               //计算偏差变化率的隶属度
  
  DirP_Member.Set_1 = Fuzzy_DirP_Rule[E_Member.Set_1][EC_Member.Set_1];         //查询规则表
  DirP_Member.Set_2 = Fuzzy_DirP_Rule[E_Member.Set_1][EC_Member.Set_2];
  DirP_Member.Set_3 = Fuzzy_DirP_Rule[E_Member.Set_2][EC_Member.Set_1];
  DirP_Member.Set_4 = Fuzzy_DirP_Rule[E_Member.Set_2][EC_Member.Set_2];
  
  DirP_Member.Set_1_Member = E_Member.Set_1_Member * EC_Member.Set_1_Member;    //计算输出隶属度1
  DirP_Member.Set_2_Member = E_Member.Set_1_Member * EC_Member.Set_2_Member;    //计算输出隶属度2
  DirP_Member.Set_3_Member = E_Member.Set_2_Member * EC_Member.Set_1_Member;    //计算输出隶属度3
  DirP_Member.Set_4_Member = E_Member.Set_2_Member * EC_Member.Set_2_Member;    //计算输出隶属度4
                                                                                //计算P的量化级别
  DirP_Value = (DirP_Member.Set_1) * DirP_Member.Set_1_Member + (DirP_Member.Set_2) * DirP_Member.Set_2_Member + (DirP_Member.Set_3) * DirP_Member.Set_3_Member + (DirP_Member.Set_4) * DirP_Member.Set_4_Member;
  
  return (DirP_Value / 6.0);
}

/*
* 自适应模糊PID--D
* 动态调整D的大小
*/
float Fuzzy_DirD(float e, float ec)
{
  float DirD_Value = 0.0;                         //输出值--P的量化级别 

  LinearQuantization(e, ec);                       //计算量化级别
  E_Membership(e);                                 //计算偏差的隶属度
  EC_Membership(ec);                               //计算偏差变化率的隶属度
  
  DirD_Member.Set_1 = Fuzzy_DirD_Rule[E_Member.Set_1][EC_Member.Set_1];         //查询规则表
  DirD_Member.Set_2 = Fuzzy_DirD_Rule[E_Member.Set_1][EC_Member.Set_2];
  DirD_Member.Set_3 = Fuzzy_DirD_Rule[E_Member.Set_2][EC_Member.Set_1];
  DirD_Member.Set_4 = Fuzzy_DirD_Rule[E_Member.Set_2][EC_Member.Set_2];
  
  DirD_Member.Set_1_Member = E_Member.Set_1_Member * EC_Member.Set_1_Member;    //计算输出隶属度1
  DirD_Member.Set_2_Member = E_Member.Set_1_Member * EC_Member.Set_2_Member;    //计算输出隶属度2
  DirD_Member.Set_3_Member = E_Member.Set_2_Member * EC_Member.Set_1_Member;    //计算输出隶属度3
  DirD_Member.Set_4_Member = E_Member.Set_2_Member * EC_Member.Set_2_Member;    //计算输出隶属度4
                                                                                //计算P的量化级别
  DirD_Value = (DirD_Member.Set_1 * DirD_Member.Set_1_Member + (DirD_Member.Set_2) * DirD_Member.Set_2_Member + (DirD_Member.Set_3) * DirD_Member.Set_3_Member + (DirD_Member.Set_4) * DirD_Member.Set_4_Member);
  
  return (DirD_Value / 6.0);
}

/*
* 线性量化函数
* 计算偏差和偏差变化率的量化级别
*/
void LinearQuantization(float e, float ec)
{
  E_Value = 6.0 * e / (e_Max);
  EC_Value = 6.0 * ec / (e_Max);
}

/*
* 偏差隶属度函数
*/
void E_Membership(float e)
{
  if(e >= -6 && e < -4)
  {
    E_Member.Set_1 = NB;
    E_Member.Set_2 = NM;
    E_Member.Set_1_Member = -0.5 * e - 2.0;
    E_Member.Set_2_Member = 0.5 * e + 3.0;
  }
  else if(e >= -4 && e < -2)
  {
    E_Member.Set_1 = NM;
    E_Member.Set_2 = NS;
    E_Member.Set_1_Member = -0.5 * e - 1.0;
    E_Member.Set_2_Member = 0.5 * e + 2.0;
  }
  else if(e >= -2 && e < 0)
  {
    E_Member.Set_1 = NS;
    E_Member.Set_2 = ZO;
    E_Member.Set_1_Member = -0.5 * e;
    E_Member.Set_2_Member = 0.5 * e + 1.0;
  }
  else if(e >= 0 && e < 2)
  {
    E_Member.Set_1 = ZO;
    E_Member.Set_2 = PS;
    E_Member.Set_1_Member = -0.5 * e + 1.0;
    E_Member.Set_2_Member = 0.5 * e;
  }
  else if(e >= 2 && e < 4)
  {
    E_Member.Set_1 = PS;
    E_Member.Set_2 = PM;
    E_Member.Set_1_Member = -0.5 * e + 2.0;
    E_Member.Set_2_Member = 0.5 * e - 1.0;
  }
  else if(e >= 4 && e <= 6)
  {
    E_Member.Set_1 = PM;
    E_Member.Set_2 = PB;
    E_Member.Set_1_Member = -0.5 * e + 3.0;
    E_Member.Set_2_Member = 0.5 * e - 2.0;
  }
}

/*
* 偏差变化率隶属度函数
*/
void EC_Membership(float ec)
{
  if(ec >= -6 && ec < -4)
  {
    EC_Member.Set_1 = NB;
    EC_Member.Set_2 = NM;
    EC_Member.Set_1_Member = -0.5 * ec - 2.0;
    EC_Member.Set_2_Member = 0.5 * ec + 3.0;
  }
  else if(ec >= -4 && ec < -2)
  {
    EC_Member.Set_1 = NM;
    EC_Member.Set_2 = NS;
    EC_Member.Set_1_Member = -0.5 * ec - 1.0;
    EC_Member.Set_2_Member = 0.5 * ec + 2.0;
  }
  else if(ec >= -2 && ec < 0)
  {
    EC_Member.Set_1 = NS;
    EC_Member.Set_2 = ZO;
    EC_Member.Set_1_Member = -0.5 * ec;
    EC_Member.Set_2_Member = 0.5 * ec + 1.0;
  }
  else if(ec >= 0 && ec < 2)
  {
    EC_Member.Set_1 = ZO;
    EC_Member.Set_2 = PS;
    EC_Member.Set_1_Member = -0.5 * ec + 1.0;
    EC_Member.Set_2_Member = 0.5 * ec;
  }
  else if(ec >= 2 && ec < 4)
  {
    EC_Member.Set_1 = PS;
    EC_Member.Set_2 = PM;
    EC_Member.Set_1_Member = -0.5 * ec + 2.0;
    EC_Member.Set_2_Member = 0.5 * ec - 1.0;
  }
  else if(ec >= 4 && ec <= 6)
  {
    EC_Member.Set_1 = PM;
    EC_Member.Set_2 = PB;
    EC_Member.Set_1_Member = -0.5 * ec + 3.0;
    EC_Member.Set_2_Member = 0.5 * ec - 2.0;
  }
}






























//#define PB  6
//#define PM  5
//#define PS  4
//#define ZO  3
//#define NS  2
//#define NM  1
//#define NB  0
//float Fuzzy(float E, float EC)
//{
//    /*输入量P语言值特征点*/
//    float EFF[7] = { -21,-14,-7,0,7,14,21 };
//    /*输入量D语言值特征点*/
//    float DFF[7] = { -12,-7,-3,0,3,7,12 };
//    /*输出量U语言值特征点(根据赛道类型选择不同的输出值)*/
//    float UFF[7] = { 0,4,9,15,23,31,40 };
//
//    int rule[7][7] = {
//        //    0  1    2  3    4   5   6
//            { 6 , 5 , 4 , 3 , 2 , 1 , 0},//0
//            { 5 , 4 , 3 , 2 , 1 , 0 , 1},//1
//            { 4 , 3 , 2 , 1 , 0 , 1 , 2},//2
//            { 3 , 2 , 1 , 0 , 1 , 2 , 3},//3
//            { 2 , 1 , 0 , 1 , 2 , 3 , 4},//4
//            { 1 , 0 , 1 , 2 , 3 , 4 , 5},//5
//            { 0 , 1 , 2 , 3 , 4 , 5 , 6},//6
//    };
//
//    //     int rule[7][7]={
//    //    //    0  1    2  3    4   5   6
//    //        { PB , PB , PM , PM , PS , ZO , ZO},//0
//    //        { PB , PB , PM , PS , PS , ZO , NS},//1
//    //        { PB , PM , PM , PS , ZO , NS , NS},//2
//    //        { PM , PM , PS , ZO , NS , NM , NM},//3
//    //        { PS , PS , ZO , NS , NS , NM , NB},//4
//    //        { PS , ZO , NS , NM , NM , NM , NB},//5
//    //        { ZO , ZO , NM , NM , NM , NB , NB},//6
//    //    };
//
//    float U = 0;  /*偏差,偏差微分以及输出值的精确量*/
//    float PF[2] = { 0 }, DF[2] = { 0 }, UF[4] = { 0 };
//    /*偏差,偏差微分以及输出值的隶属度*/
//    int Pn = 0, Dn = 0, Un[4] = { 0 };
//    float t1 = 0, t2 = 0, t3 = 0, t4 = 0, temp1 = 0, temp2 = 0;
//    /*隶属度的确定*/
//    /*根据PD的指定语言值获得有效隶属度*/
//    if (E > EFF[0] && E < EFF[6])
//    {
//        if (E <= EFF[1])
//        {
//            Pn = -2;
//            PF[0] = (EFF[1] - E) / (EFF[1] - EFF[0]);
//        }
//        else if (E <= EFF[2])
//        {
//            Pn = -1;
//            PF[0] = (EFF[2] - E) / (EFF[2] - EFF[1]);
//        }
//        else if (E <= EFF[3])
//        {
//            Pn = 0;
//            PF[0] = (EFF[3] - E) / (EFF[3] - EFF[2]);
//        }
//        else if (E <= EFF[4])
//        {
//            Pn = 1;
//            PF[0] = (EFF[4] - E) / (EFF[4] - EFF[3]);
//        }
//        else if (E <= EFF[5])
//        {
//            Pn = 2;
//            PF[0] = (EFF[5] - E) / (EFF[5] - EFF[4]);
//        }
//        else if (E <= EFF[6])
//        {
//            Pn = 3;
//            PF[0] = (EFF[6] - E) / (EFF[6] - EFF[5]);
//        }
//    }
//
//    else if (E <= EFF[0])
//    {
//        Pn = -2;
//        PF[0] = 1;
//    }
//    else if (E >= EFF[6])
//    {
//        Pn = 3;
//        PF[0] = 0;
//    }
//
//    PF[1] = 1 - PF[0];
//
//
//    //判断D的隶属度
//    if (EC > DFF[0] && EC < DFF[6])
//    {
//        if (EC <= DFF[1])
//        {
//            Dn = -2;
//            DF[0] = (DFF[1] - EC) / (DFF[1] - DFF[0]);
//        }
//        else if (EC <= DFF[2])
//        {
//            Dn = -1;
//            DF[0] = (DFF[2] - EC) / (DFF[2] - DFF[1]);
//        }
//        else if (EC <= DFF[3])
//        {
//            Dn = 0;
//            DF[0] = (DFF[3] - EC) / (DFF[3] - DFF[2]);
//        }
//        else if (EC <= DFF[4])
//        {
//            Dn = 1;
//            DF[0] = (DFF[4] - EC) / (DFF[4] - DFF[3]);
//        }
//        else if (EC <= DFF[5])
//        {
//            Dn = 2;
//            DF[0] = (DFF[5] - EC) / (DFF[5] - DFF[4]);
//        }
//        else if (EC <= DFF[6])
//        {
//            Dn = 3;
//            DF[0] = (DFF[6] - EC) / (DFF[6] - DFF[5]);
//        }
//    }
//    //不在给定的区间内
//    else if (EC <= DFF[0])
//    {
//        Dn = -2;
//        DF[0] = 1;
//    }
//    else if (EC >= DFF[6])
//    {
//        Dn = 3;
//        DF[0] = 0;
//    }
//
//    DF[1] = 1 - DF[0];
//
//    /*使用误差范围优化后的规则表rule[7][7]*/
//    /*输出值使用13个隶属函数,中心值由UFF[7]指定*/
//    /*一般都是四个规则有效*/
//    Un[0] = rule[Pn + 2][Dn + 2];
//    Un[1] = rule[Pn + 3][Dn + 2];
//    Un[2] = rule[Pn + 2][Dn + 3];
//    Un[3] = rule[Pn + 3][Dn + 3];
//
//    if (PF[0] <= DF[0])    //求小
//        UF[0] = PF[0];
//    else
//        UF[0] = DF[0];
//    if (PF[1] <= DF[0])
//        UF[1] = PF[1];
//    else
//        UF[1] = DF[0];
//    if (PF[0] <= DF[1])
//        UF[2] = PF[0];
//    else
//        UF[2] = DF[1];
//    if (PF[1] <= DF[1])
//        UF[3] = PF[1];
//    else
//        UF[3] = DF[1];
//    /*同隶属函数输出语言值求大*/
//    if (Un[0] == Un[1])
//    {
//        if (UF[0] > UF[1])
//            UF[1] = 0;
//        else
//            UF[0] = 0;
//    }
//    if (Un[0] == Un[2])
//    {
//        if (UF[0] > UF[2])
//            UF[2] = 0;
//        else
//            UF[0] = 0;
//    }
//    if (Un[0] == Un[3])
//    {
//        if (UF[0] > UF[3])
//            UF[3] = 0;
//        else
//            UF[0] = 0;
//    }
//    if (Un[1] == Un[2])
//    {
//        if (UF[1] > UF[2])
//            UF[2] = 0;
//        else
//            UF[1] = 0;
//    }
//    if (Un[1] == Un[3])
//    {
//        if (UF[1] > UF[3])
//            UF[3] = 0;
//        else
//            UF[1] = 0;
//    }
//    if (Un[2] == Un[3])
//    {
//        if (UF[2] > UF[3])
//            UF[3] = 0;
//        else
//            UF[2] = 0;
//    }
//    t1 = UF[0] * UFF[Un[0]];
//    t2 = UF[1] * UFF[Un[1]];
//    t3 = UF[2] * UFF[Un[2]];
//    t4 = UF[3] * UFF[Un[3]];
//    temp1 = t1 + t2 + t3 + t4;
//    temp2 = UF[0] + UF[1] + UF[2] + UF[3];//模糊量输出
//    U = temp1 / temp2;
//    return U;
//}
///*******************************************************************************************************************************/
//float Fuzzy_D(float E, float EC)
//{
//    /*输入量P语言值特征点*/
//    float EFF[7] = { -21,-14,-7,0,7,14,21 };
//    /*输入量D语言值特征点*/
//    float DFF[7] = { -12,-7,-3,0,3,7,12 };
//    /*输出量U语言值特征点(根据赛道类型选择不同的输出值)*/
//    float UFF[7] = { 0,4,8,12,18,26,36 };
//
//    int rule[7][7] = {
//        //    0  1    2  3    4   5   6
//            { 6 , 1 , 2 , 3 , 4 , 5 , 6},//0
//            { 1 , 2 , 3 , 4 , 5 , 6 , 5},//1
//            { 2 , 3 , 4 , 5 , 6 , 5 , 4},//2
//            { 3 , 4 , 5 , 6 , 5 , 4 , 3},//3
//            { 4 , 5 , 6 , 5 , 4 , 3 , 2},//4
//            { 5 , 6 , 5 , 4 , 3 , 2 , 1},//5
//            { 6 , 5 , 4 , 3 , 2 , 1 , 0},//6
//    };
//
//    //     int rule[7][7]={
//    //    //    0  1    2  3    4   5   6
//    //        { PB , PB , PM , PM , PS , ZO , ZO},//0
//    //        { PB , PB , PM , PS , PS , ZO , NS},//1
//    //        { PB , PM , PM , PS , ZO , NS , NS},//2
//    //        { PM , PM , PS , ZO , NS , NM , NM},//3
//    //        { PS , PS , ZO , NS , NS , NM , NB},//4
//    //        { PS , ZO , NS , NM , NM , NM , NB},//5
//    //        { ZO , ZO , NM , NM , NM , NB , NB},//6
//    //    };
//
//    float U = 0;  /*偏差,偏差微分以及输出值的精确量*/
//    float PF[2] = { 0 }, DF[2] = { 0 }, UF[4] = { 0 };
//    /*偏差,偏差微分以及输出值的隶属度*/
//    int Pn = 0, Dn = 0, Un[4] = { 0 };
//    float t1 = 0, t2 = 0, t3 = 0, t4 = 0, temp1 = 0, temp2 = 0;
//    /*隶属度的确定*/
//    /*根据PD的指定语言值获得有效隶属度*/
//    if (E > EFF[0] && E < EFF[6])
//    {
//        if (E <= EFF[1])
//        {
//            Pn = -2;
//            PF[0] = (EFF[1] - E) / (EFF[1] - EFF[0]);
//        }
//        else if (E <= EFF[2])
//        {
//            Pn = -1;
//            PF[0] = (EFF[2] - E) / (EFF[2] - EFF[1]);
//        }
//        else if (E <= EFF[3])
//        {
//            Pn = 0;
//            PF[0] = (EFF[3] - E) / (EFF[3] - EFF[2]);
//        }
//        else if (E <= EFF[4])
//        {
//            Pn = 1;
//            PF[0] = (EFF[4] - E) / (EFF[4] - EFF[3]);
//        }
//        else if (E <= EFF[5])
//        {
//            Pn = 2;
//            PF[0] = (EFF[5] - E) / (EFF[5] - EFF[4]);
//        }
//        else if (E <= EFF[6])
//        {
//            Pn = 3;
//            PF[0] = (EFF[6] - E) / (EFF[6] - EFF[5]);
//        }
//    }
//
//    else if (E <= EFF[0])
//    {
//        Pn = -2;
//        PF[0] = 1;
//    }
//    else if (E >= EFF[6])
//    {
//        Pn = 3;
//        PF[0] = 0;
//    }
//
//    PF[1] = 1 - PF[0];
//
//
//    //判断D的隶属度
//    if (EC > DFF[0] && EC < DFF[6])
//    {
//        if (EC <= DFF[1])
//        {
//            Dn = -2;
//            DF[0] = (DFF[1] - EC) / (DFF[1] - DFF[0]);
//        }
//        else if (EC <= DFF[2])
//        {
//            Dn = -1;
//            DF[0] = (DFF[2] - EC) / (DFF[2] - DFF[1]);
//        }
//        else if (EC <= DFF[3])
//        {
//            Dn = 0;
//            DF[0] = (DFF[3] - EC) / (DFF[3] - DFF[2]);
//        }
//        else if (EC <= DFF[4])
//        {
//            Dn = 1;
//            DF[0] = (DFF[4] - EC) / (DFF[4] - DFF[3]);
//        }
//        else if (EC <= DFF[5])
//        {
//            Dn = 2;
//            DF[0] = (DFF[5] - EC) / (DFF[5] - DFF[4]);
//        }
//        else if (EC <= DFF[6])
//        {
//            Dn = 3;
//            DF[0] = (DFF[6] - EC) / (DFF[6] - DFF[5]);
//        }
//    }
//    //不在给定的区间内
//    else if (EC <= DFF[0])
//    {
//        Dn = -2;
//        DF[0] = 1;
//    }
//    else if (EC >= DFF[6])
//    {
//        Dn = 3;
//        DF[0] = 0;
//    }
//
//    DF[1] = 1 - DF[0];
//
//    /*使用误差范围优化后的规则表rule[7][7]*/
//    /*输出值使用13个隶属函数,中心值由UFF[7]指定*/
//    /*一般都是四个规则有效*/
//    Un[0] = rule[Pn + 2][Dn + 2];
//    Un[1] = rule[Pn + 3][Dn + 2];
//    Un[2] = rule[Pn + 2][Dn + 3];
//    Un[3] = rule[Pn + 3][Dn + 3];
//
//    if (PF[0] <= DF[0])    //求小
//        UF[0] = PF[0];
//    else
//        UF[0] = DF[0];
//    if (PF[1] <= DF[0])
//        UF[1] = PF[1];
//    else
//        UF[1] = DF[0];
//    if (PF[0] <= DF[1])
//        UF[2] = PF[0];
//    else
//        UF[2] = DF[1];
//    if (PF[1] <= DF[1])
//        UF[3] = PF[1];
//    else
//        UF[3] = DF[1];
//    /*同隶属函数输出语言值求大*/
//    if (Un[0] == Un[1])
//    {
//        if (UF[0] > UF[1])
//            UF[1] = 0;
//        else
//            UF[0] = 0;
//    }
//    if (Un[0] == Un[2])
//    {
//        if (UF[0] > UF[2])
//            UF[2] = 0;
//        else
//            UF[0] = 0;
//    }
//    if (Un[0] == Un[3])
//    {
//        if (UF[0] > UF[3])
//            UF[3] = 0;
//        else
//            UF[0] = 0;
//    }
//    if (Un[1] == Un[2])
//    {
//        if (UF[1] > UF[2])
//            UF[2] = 0;
//        else
//            UF[1] = 0;
//    }
//    if (Un[1] == Un[3])
//    {
//        if (UF[1] > UF[3])
//            UF[3] = 0;
//        else
//            UF[1] = 0;
//    }
//    if (Un[2] == Un[3])
//    {
//        if (UF[2] > UF[3])
//            UF[3] = 0;
//        else
//            UF[2] = 0;
//    }
//    t1 = UF[0] * UFF[Un[0]];
//    t2 = UF[1] * UFF[Un[1]];
//    t3 = UF[2] * UFF[Un[2]];
//    t4 = UF[3] * UFF[Un[3]];
//    temp1 = t1 + t2 + t3 + t4;
//    temp2 = UF[0] + UF[1] + UF[2] + UF[3];//模糊量输出
//    U = temp1 / temp2;
//    return U;
//}
