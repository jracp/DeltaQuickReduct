// Programmed by Javad Rahimipour Anaraki on 22/08/16 updated on 25/01/18
// Ph.D. Candidate
// Department of Computer Science
// Memorial University of Newfoundland
// jra066 [AT] mun [DOT] ca | www.cs.mun.ca/~jra066

// This is an implementation of Delta Quick Reduct algorithm
// Input: dataset name in the same path
// Output: is a subset of selected features
// More info: https://link.springer.com/chapter/10.1007/978-3-662-53611-7_1

#include <iostream>
#include <stdio.h>
#include <cmath>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <cstring>
#include <map>

using namespace std;

double dependency(int r, int c, int nCls, int cls[], double v[], vector<vector<double> >& data, int maxF[], int lF);

int main(int argc, char *argv[])
{
    string value, elmnt, temp;
    string path = argv[1]; //Dataset name with sorted classification outcome (ascen)
    ofstream out_data(path + "_results.txt"); //Output file
    ifstream dataset (path); //Open dataset file as an input
    ifstream tData (path); //A temp input
    int r = 0, c = 0, pos = 0, i = 0, j = 0, pStart = 0, pEnd = 0;

    out_data << path << endl; //Add input filename to the output file

    //Calcultating number of rows and columns
    getline (dataset, value);
    pos = value.find(",");
    while(pos!=-1) {
        value.replace(pos, 1, "0");
        pos = value.find(",");
        c++;
    }
    c++;
    
    while(dataset.good()) {
        getline (dataset, value);
        r++;
    }
    
    //Converting dataset into a matrix
    vector<double> tempData(c);
    vector< vector<double> > data(r, tempData);
    
    for (i=0;i<r;++i) {
        getline (tData, elmnt);
        pStart = 0;
        pEnd = elmnt.find(",");
        j = 0;
        
        while(pEnd != -1) {
            temp = elmnt.substr(pStart, pEnd - pStart);
            data[i][j]=atof(temp.c_str());
            elmnt.replace(pEnd, 1, "_");
            pStart = pEnd + 1;
            pEnd = elmnt.find(",");
            j++;
        }
        temp = elmnt.substr(pStart, 1);
        data[i][j]=atof(temp.c_str());
    }
    
    //Finding number of classes in dataset
    int cls[c], clsTmp;
    int nCls = 1;
    j = 0;
    clsTmp = data[0][c-1];
    cls[j] = 0;
    
    for (i=1;i<r;++i) {
        if (clsTmp != data[i][c-1]) {
            j++;
            cls[j] = i;
            clsTmp = data[i][c-1];
            nCls++;
        }
    }
    
    for (j=0;j<nCls-1;++j) {
        cls[j] = cls[j+1];
    }
    cls[nCls-1] = r;
    
    //Calculating variance of the features
    double sum = 0.0;
    double mean = 0.0, v[c-1];
    
    for (j=0;j<c-1;++j) {
        sum = 0.0;
        for (i=0;i<r;++i) {
            sum += data[i][j];
        }
        mean = sum / r;
        v[j] = 0.0;
        
        for (i=0;i<r;i++) {
            v[j] = v[j] + pow((data[i][j] - mean), 2.0);
        }
        v[j] = pow(v[j]/(r - 1), 0.5);
    }
    
    int selF[c], biF[2];
    double maxDD = 0.0, cDD = 0.0, depFF = 0.0, FM = 0.0;
    bool crit = true;
    int cnt = 1, maxF = 0, f;
    vector<double> bdata(2);
    vector<vector<double> > biData(r, bdata);
    vector<vector<double> > fiData(r, bdata);
    map <int, int> fMap;
    pair <int, int> fPair;
    
    while(crit) {
        for (f=0;f<c-1;++f) {
            //Check to see if the current feature has already selected
            fPair.first = f;
            fPair.second = f;
            if(fMap.insert(fPair).second == true) {continue;}
            
            selF[cnt-1] = f;
            if (cnt == 1) {
                cDD = dependency(r, c, nCls, cls, v, data, selF, cnt);
            } else {
                depFF = 0.0;
                cDD = 0.0;
                biF[1] = f;
                for (int b=0;b<cnt-1;++b) {
                    //Preparing the data
                    for (int rw=0;rw<r;rw++) {
                        biData[rw][0] = data[rw][selF[b]]; //For calculating correlation between features, themselves
                        biData[rw][1] = data[rw][f]; //Newly added feature
                        fiData[rw][0] = data[rw][selF[b]]; //For calculating correlation between features and class
                        fiData[rw][1] = data[rw][c-1]; //Class attribute
                    }
                    
                    //Sorting based on feature "f"
                    double tmp0, tmp1 = 0.0;
                    for (i=0;i<r-1;++i) {
                        for (int j=i+1;j<r;++j) {
                            if (biData[i][1]>biData[j][1]) {
                                tmp0 = biData[i][0];
                                tmp1 = biData[i][1];
                                biData[i][0]= biData[j][0];
                                biData[i][1] = biData[j][1];
                                biData[j][0] = tmp0;
                                biData[j][1] = tmp1;
                            }
                        }
                    }
                    
                    //Finding number of classes for feature "f"
                    int clsF[r], clsTmp;
                    int nClsF = 1;
                    int k = 0;
                    clsTmp = biData[0][1];
                    clsF[k] = 0;
                    
                    biF[0] = selF[b];
                    
                    for (i=1;i<r;i++) {
                        if (clsTmp != biData[i][1]) {
                            k++;
                            clsF[k] = i;
                            clsTmp = biData[i][1];
                            nClsF++;
                        }
                    }
                    double tmpV[1] = {0.0};
                    tmpV[0] = v[selF[b]];
                    int tmpF[1] = {0};
                    tmpF[0] = 0;
                    depFF += dependency(r, 2, nClsF, clsF, tmpV, biData, tmpF, 1);
                    cDD += dependency(r, 2, nCls, cls, tmpV, fiData, tmpF, 1);
                }
            }
            
            FM = cDD/pow((cnt+(cnt*depFF)), 0.5);
            
            if (FM > maxDD) {
                maxDD = FM;
                maxF = f;
            }
        }
        
        if ((cnt == c) || ((cnt > 1) && (selF[cnt-2] == maxF))) {
            crit = false;
        } else {
            cout << "F = " << maxF+1 << " > " << FM << endl;
            if (cnt==1) { maxDD = 0.0;}
            selF[cnt-1] = maxF;
            ++cnt;
        }
    }

    cout << endl;
    printf("[ ");
    out_data << "[ "; //add to file
    
    for (int it=0;it<cnt-1;it++) {
        printf("%d ", selF[it] + 1);
        out_data << selF[it] + 1 << ","; //add to file
    }
    
    printf ("] \nFM = %f \n", FM);
    cout << endl;
    out_data << "]" << endl << "FM =" << maxDD << endl << endl; //add to file
    out_data.close();
    return 0;
}

double dependency(int r, int c, int nCls, int cls[], double v[], vector<vector<double> >& data, int maxF[], int lF)
{
    int mF = 1, h = 0, k = 0, s = 0, i = 0, j = 0, biF = 0;
    int lMoD = nCls;
    double fterm1 = 0.0, fterm2 = 0.0, moRp = 0.0, out = 0.0;
    double tmp[lMoD], moRa[lF], supMat[r];
    vector<double> tmpMoD(2 * lMoD);
    vector< vector<double> > moX(r, tmpMoD);
    
    for (int a=0;a<r;++a) {
        for (int b=0;b<(2*lMoD);++b) {
            moX[a][b] = 0.0;
        }
    }
    
    for (nCls=0;nCls<lMoD;++nCls) {
        for (s;s<cls[nCls];++s) {
            moX[s][nCls] = 1.0;
        }
        s = cls[nCls];
    }
    
    for (int x1=0;x1<r;++x1) {
        for (int x2=0;x2<r;++x2) {
            for (h=0;h<lF;++h) {
                mF = maxF[h];
                fterm1 = (data[x2][mF] - (data[x1][mF] - v[mF])) / v[mF];
                fterm2 = ((data[x1][mF] + v[mF]) - data[x2][mF]) / v[mF];
                moRa[h] = max(min(fterm1, fterm2), 0.0);
            }
            
            if (lF > 1) {
                moRp = max(moRa[0] + moRa[1] - 1.0, 0.0);
                for(int nMoRmF=2;nMoRmF<lF;++nMoRmF) {
                    moRp = max(moRp + moRa[nMoRmF] - 1.0, 0.0);
                }
            } else {
                moRp = moRa[0];
            }
            
            for (int j=lMoD;j<(2*lMoD);++j) {
                moX[x2][j] = min(1- moRp + moX[x2][j-lMoD], 1.0);
            }
        }
        
        k = 0;
        
        for (j=lMoD;j<(2*lMoD);++j) {
            tmp[k] = moX[0][j];
            for (i=1;i<r;++i) {
                tmp[k] = min(moX[i][j], tmp[k]);
            }
            k++;
        }
        
        supMat[x1] = tmp[0];
        
        for (k=1;k<lMoD;++k) {
            supMat[x1] = max(tmp[k], supMat[x1]);
        }
    }
    
    for (k=0;k<r;++k) {
        out += supMat[k];
        supMat[k] = 0.0;
    }
    
    out = out / r;
    return out;
}
