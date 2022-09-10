#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <iomanip>
using namespace std;

const int A_VAL = 0;
const int B_VAL = 1;
const int C_VAL = 2;
const int D_VAL = 3;
const int E_VAL = 4;

int GetLetterVal(double score, double average) //Determines the letter grade based on its displacement from the average
{
    int letterVal = -1;
    if (score > average) {
          if (score <= average + 5.0)
          {
              letterVal = C_VAL;
          }
          else if (score <= average + 15.0)
          {
              letterVal = B_VAL;
          }
          else
          {
              letterVal = A_VAL;
          }
      }
      else
      {
          if (score >= average - 5.0)
          {
              letterVal = C_VAL;
          }
          else if (score >= average - 15.0)
          {
              letterVal = D_VAL;
          }
          else
          {
              letterVal = E_VAL;
          }
      }
    return letterVal;
}

char GetLetterChar(int letterVal) //Translates the int value of letterVal to a char
{
    if (letterVal == A_VAL) return 'A';
    else if (letterVal == B_VAL) return 'B';
    else if (letterVal == C_VAL) return 'C';
    else if (letterVal == D_VAL) return 'D';
    else if (letterVal == E_VAL) return 'E';
    else return '0';
}

int main(int argc, char* argv[])
{
    const int SCORES_WIDTH = 4;
    const int NAMES_WIDTH = 20;
    const int NUM_LETTERS = 5;
    
    //Checking for correct number of arguments
    if (argc < 3)
    {
       cerr << "Please provide the name of input and output files, thanks\n";
       return 1;
    }
    
    //Checking if input file opens
    ifstream in(argv[1]);
    if (!in)
    {
        cerr << "Failure to open input file: " << argv[1] << endl;
        return 2;
    }
    
    //Checking if output file opens
    ofstream out(argv[2]);
    if (!out)
    {
        in.close();
        cerr << "Failure to open output file: " << argv[1] << endl;
        return 3;
    }
    
    //Reading number of exams and students from file
    int numStudents = 0;
    int numExams = 0;
    in >> numStudents >> numExams;
    in.ignore();
   
    //Creating Dynamic 2D array of exam scores
    double** examScores = new double*[numStudents];
    for (int row = 0; row < numStudents; row++)
    {
        examScores[row] = new double[numExams];
    }
    
    //Reads in the full student name for each line
    string* nameList = new string[numStudents];
    for (int row = 0; row < numStudents; row++)
    {
        string line = " ";
        getline(in, line);
        size_t p = 0;
        while(!isdigit(line[p])) ++p;
        nameList[row] = line.substr(0,p);
        istringstream iss(line.substr(p, line.size() - p));
        
        //Reading in the grades for each line
        for (int col = 0; col < numExams; col++)
        {
            int grade = 0;
            iss >> grade;
            examScores[row][col] = grade;
        }
    }
    
    //Outputting student scores formatted more nicely
    out << "Student Scores: \n";
    for (int row = 0; row < numStudents; row++)
    {
        out << setw(NAMES_WIDTH) << nameList[row] << " ";
        for (int col = 0; col < numExams; col++)
        {
            out << fixed << setprecision(0) << setw(SCORES_WIDTH) << examScores[row][col];
        }
        out << endl;
    }
    
    //Output exam averages
    double* averageList = new double[numExams];
    out << "Exam Averages: \n";
    double sum = 0.0;
    for (int col = 0; col < numExams; col++)
    {
        for (int row = 0; row < numStudents; row++)
        {
            sum += examScores[row][col];
        }
        averageList[col] = sum / numStudents;
        out << setw(NAMES_WIDTH) << "Exam " << col + 1 << " Average =";
        out << setw(SCORES_WIDTH) << setprecision(1) << averageList[col] << endl;
        sum = 0.0;
    }
    
    //Making an array of letter grades
    int** numGradeList = new int*[numExams];
    for (int col = 0; col < numExams; col++)
    {
        numGradeList[col] = new int[NUM_LETTERS];
    }
    
    //Initializing the array
    for (int col = 0; col < numExams; col++)
    {
        for (int letter = 0; letter < NUM_LETTERS; letter++)
        {
            numGradeList[col][letter] = 0;
        }
    }
    
    //Calculating letter grades
    char letterGrade = '0';
    int letterVal = -1; //translating char into int
    out << "Student Exam Grades:";
    for (int row = 0; row < numStudents; row++)
    {
        out << endl << setw(NAMES_WIDTH) << nameList[row];
        for (int col = 0; col < numExams; col++)
        {
            letterVal = GetLetterVal(examScores[row][col], averageList[col]);
            letterGrade = GetLetterChar(letterVal);
            out << setw(SCORES_WIDTH) << setprecision(0) << examScores[row][col] << " (" << letterGrade << ") ";
            ++numGradeList[col][letterVal];
            letterVal = -100; //reseting letter value so errors are easy to detect
            }
        }
    
    //Outputting letter grades for exams overall
    out << "\nExam Grades: \n";
    for (int col = 0; col < numExams; col++)
    {
        out << setw(SCORES_WIDTH) << "Exam " << col + 1; 
        out << setw(SCORES_WIDTH) << numGradeList[col][A_VAL] << "(A)";
        out << setw(SCORES_WIDTH) << numGradeList[col][B_VAL] << "(B)";
        out << setw(SCORES_WIDTH) << numGradeList[col][C_VAL] << "(C)";
        out << setw(SCORES_WIDTH) << numGradeList[col][D_VAL] << "(D)";
        out << setw(SCORES_WIDTH) << numGradeList[col][E_VAL] << "(E)";
        out << endl;
    }
    
    //Calculate class final average
    double classAverage = 0.0;
    for (int row = 0; row < numStudents; row++)
    {
        for (int col = 0; col < numExams; col++)
        {
            classAverage += examScores[row][col];
        }
    }
    classAverage /= (numExams * numStudents);
    
    //Creating array of student final grades
    double* finalGrades = new double[numStudents];
    
    //Calculating student average grades
    sum = 0.0;
    for (int row = 0; row < numStudents; row++)
    {
        for (int col = 0; col < numExams; col++)
        {
            sum += examScores[row][col];
        }
        finalGrades[row] = sum / numExams;
        sum = 0.0;
    }
    
    //Output student final grades
    out << "Student Final Grades:\n";
    for (int row = 0; row < numStudents; row++)
    {
        out << setw(NAMES_WIDTH) <<  nameList[row];
        out << setw(SCORES_WIDTH) << setprecision(1) << finalGrades[row] << " (" << GetLetterChar(GetLetterVal(finalGrades[row], classAverage)) << ")\n";
    }
    out << setw(NAMES_WIDTH) << "Class Average Score = " << classAverage << endl;
    
    //Freeing memory from examScores
    for (int i = 0; i < numStudents; ++i)
    {
        delete [] examScores[i];
    }
    delete [] examScores;
    
    //Freeing memory from nameList
    delete [] nameList;
    
    //Freeing memory from averageList
    delete [] averageList;
    
    //Freeing memory from numGradesList
    for (int i = 0; i < numExams; i++)
    {
        delete [] numGradeList[i];
    }
    delete [] numGradeList;
 
    //Freeing memory from finalGrades
    delete [] finalGrades;
    
    return 0;
}
