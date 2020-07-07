#include<fstream.h> //for data file handling.
#include<conio.h>   //for clrscr() and getch() functions
#include<string.h>  //for string functions like strcpy()
#include<dos.h>     //for delay() function
#include<stdlib.h>  //for random() and exit() functions
#include<stdio.h>   //for gets() function

fstream ffile,Questions, POOL;

int pool_no=0;

//function prototypes
void notffile();
void createpool();
void browse();
void display_details();
int Enterpassword();
void reviewmistakes();
void settings();
void Exit();
void delete_question(int no);
int get_totalquestions();

class UDDT   //user-defined data type
{
protected:
	char name[40];
	int id;
public:
	UDDT()
	{
		id=-1;
	}
	char *return_name()
	{
		return name;
	}
	int return_id()
	{
		return id;
	}
	void set_name(char temp[])
	{
		strcpy(name, temp);
	}
	void set_id(int temp)
	{
		id=temp;
	}
	void clear_data()
	{
		id=-1;
		strcpy(name,"");
	}
};


struct CHOICE
{
	UDDT chosentype;
	UDDT chosenchapter;
	UDDT chosensubject;
}select;


class QUESTION
{
	char question[200];
	int marks;
	int qno;
	int mistakes;
	char solution[500];
public:
       UDDT chapter;
       UDDT type;
       char *return_question()
       {
		return question;
       }
       int return_marks()
       {
		return marks;
       }
       int return_mistakes()
       {
		return mistakes;
       }
       int return_qno()
       {
		return qno;
       }
       char *return_solution()
       {
		return solution;
       }
       void set_question(char temp[])
       {
		strcpy(question, temp);
       }
       void set_marks(int temp)
       {
		marks=temp;
       }
       void set_mistakes(int temp)
       {
		mistakes=temp;
       }
       void set_qno(int temp)
       {
		qno=temp;
       }
       void set_solution(char temp[])
       {
		strcpy(solution, temp);
       }
};

void displaydetails(QUESTION q);

//functions


void delete_question(int qn)
{
	QUESTION q;
	ifstream infile;
	ofstream newfile;
	infile.open("CHEM_Q.dat", ios::binary);
	newfile.open("temp.dat", ios::binary);
	infile.read((char*)&q, sizeof(q));
	while(!infile.eof())
	{
		if(q.return_qno()<qn)   //1
		{
			newfile.write((char*)&q, sizeof(q));
		}
		else if(q.return_qno()>qn)//2
		{
			q.set_qno(q.return_qno()-1);                    //1 and 2 are checked so that the question is deleted in such a way the order (ascending, one after another) of the qno in the file does not change
			newfile.write((char*)&q, sizeof(q));
		}
		else;
		//else if qno=selected qno,then don't copy to file
		infile.read((char*)&q, sizeof(q));
	}
	newfile.close();
	infile.close();
	remove("CHEM_Q.dat");
	rename("temp.dat", "CHEM_Q.dat");
}

int Enterpassword()
{
	clrscr();
	cout<<"Enter the password: ";
	char tpass[30];
	int g,i=0;
	do
	{
		g=getch(); // get a character input from user, and store the ascii value i
		if(g!=13) // 13 is the ascii code for ENTER key
		{
			if(g!=8)
			{
				tpass[i]=(char)g; // store the character in the array tpass
				cout<<"*"; // but display *
				i++;
			}
			else if(g==8 && i!=0) //8 is the ascii code for BACKSPACE key
			{
				cout<<"\b \b";
				i--;
			}
			else if(g==8 && i==0) //if user enters backspace when no input is there already
			{
				; //do nothing
			}
	}
	}while((int)g!=13) ;
	tpass[i]='\0'; // terminate password string with /0
	if(strcmp(tpass,"admin")==0)
		return 1;
	else
		return -1;
}

void displaydetails(QUESTION q)
{
	cout<<"Question: "<<q.return_question()<<endl;
	cout<<"Type: "<<q.type.return_name()<<endl;
	cout<<"Chapter: "<<q.chapter.return_name()<<endl;
	cout<<"qno: "<<q.return_qno()<<endl;
	cout<<"Marks: "<<q.return_marks()<<endl;
	cout<<"Mistakes: "<<q.return_mistakes()<<endl;


}


void notffile()
{
	cout<<"File Open Error! Aborting...";
	delay(2500);
	exit(0);
}

void createpool()
{
	QUESTION q;
	pool_no=0;
	POOL.open("POOL.dat", ios::binary | ios::out);
	Questions.open("CHEM_Q.dat", ios::in | ios::binary);
	if(!Questions)
	{
		notffile();
	}
	if(!POOL)
	{
		notffile();
	}
	Questions.read((char*)&q, sizeof(q));
	if(select.chosenchapter.return_id()!=-1)	//if user has chosen 'chapter' as the method of practice
	{
		while (!Questions.eof())
		{
			if(select.chosenchapter.return_id()==q.chapter.return_id())
			{
				POOL.write((char*)&q, sizeof(q));
				pool_no++;
			}
			Questions.read((char*)&q, sizeof(q));
		}
	}
	else if(select.chosentype.return_id()!=-1)	//if user has chosen 'type' as the mode of practice
	{
		while (!Questions.eof())
		{
			if(select.chosentype.return_id()==q.type.return_id())
			{
				POOL.write((char*)&q, sizeof(q));
				pool_no++;
			}
			Questions.read((char*)&q, sizeof(q));
		}
	}
	POOL.close();
	Questions.close();
}

void displayquestions()
{
	char ch;
	QUESTION q;
	POOL.open("POOL.dat", ios::in | ios::binary);
	if(!POOL)
	{
		notffile();
	}
	do
	{
		int qn;
		POOL.clear();
		clrscr();
		qn=random(pool_no);
		POOL.seekg(qn*sizeof(q), ios::beg);
		POOL.read((char*)&q, sizeof(q));
		cout<<q.type.return_name()<<":\n\n";
		cout<<q.return_question()<<"\n\n";
		cout<<"Press a key to view the solution...";
		getch();
		cout<<endl<<endl<<q.return_solution()<<endl<<endl;
		cout<<"Got it correct?(y/n): ";
		cin>>ch;
		while(ch!='y' && ch!='Y' && ch!='n' && ch!='N')
		{
			cin.clear();
			cin.ignore();
			cout<<"\nInvalid choice!";
			cout<<"\nGot it correct?(y/n): ";
			cin>>ch;
		}
		if(ch=='y'||ch=='Y')
			cout<<"Well done!\n";
		else if(ch=='n' || ch=='N')
		{
			QUESTION temp;
			ofstream outfile;
			outfile.open("temp.dat",ios::binary);
			Questions.open("CHEM_Q.dat", ios::binary | ios::in);
			Questions.read((char*)&temp,sizeof(temp));
			while(!Questions.eof())
			{
				if(q.return_qno()==temp.return_qno())
				{
					temp.set_mistakes(temp.return_mistakes()+1);
				}
				outfile.write((char*)&temp, sizeof(temp));
				Questions.read((char*)&temp,sizeof(temp));
			}
			outfile.close();
			Questions.close();
			remove("CHEM_Q.dat");
			rename("temp.dat", "CHEM_Q.dat");
		}
		cout<<"\nDo you want to try another question?(Y)\n";
		cout<<"If not, enter any other character: ";
		cin>>ch;
	}while(ch=='y' || ch=='Y');
	POOL.close();
	select.chosentype.clear_data();
	select.chosenchapter.clear_data();
}

void browse()
{
			int ch,qnum,i, N,total;
			QUESTION q;
			browse:
			Questions.open("CHEM_Q.dat", ios::in | ios::binary);
			clrscr();
			cout<<"Would you like to\n";
			cout<<"1.View all questions and their details\n";
			cout<<"2.View details of selected question(with qno)\n";
			cout<<"3. Go back\n";
			cout<<"Enter your choice: ";
			cin>>ch;
			while(!cin || ch<1 || ch>3)
			{
				cin.clear();
				cin.ignore();
				Questions.close();
				cout<<"\nInvalid choice!";
				cout<<"\nPress a key to try again...";
				getch();
				goto browse;
			}
			total=get_totalquestions();
			switch(ch)
			{

			   case 1:
				clrscr();
				N=0;
				while(N<total)
				{
					clrscr();
					for(i=0;i<2;i++)
					{
						Questions.read((char*)&q, sizeof(q));
						displaydetails(q);
						cout<<endl<<endl;
						N++;
						if(N>=total)
							break;
					}
					if(N<total)
					{
						cout<<"1. Next Page\n";
						cout<<"2. Return to menu\n";
						cout<<"Enter your choice: ";
						cin>>ch;
						while(!cin || ch<1 || ch>2)
						{
							cin.clear();
							cin.ignore();
							cout<<"\nInvalid choice!"<<endl;
							cout<<"1. Next Page\n";
							cout<<"2. Return to menu\n";
							cout<<"Enter your choice: ";
							cin>>ch;
						}
						if(ch==2)
						{
							Questions.close();
							return ;
						}
					}
					else
					{
						cout<<"All questions displayed...\n";
						cout<<"Press a key to return to menu";
						getch();
						Questions.close();
						return ;
					}
				}
			   case 2:
				int qn;
				QUESTION temp;
				clrscr();
				cout<<"Enter qno: ";
				cin>>qn;
				if(qn>=total || qn<0 || !cin)
				{
					Questions.close();
					cout<<"Question does not exit!";
					cout<<"\nPress a key to return...";
					getch();
					return ;
				}
				cout<<"\nDetails of question with qno "<<qn<<endl<<endl;
				Questions.read((char*)&temp, sizeof(temp));
				while(!Questions.eof())
				{                             //searches the file for the question with that qno and saves it in 'q'
					if (temp.return_qno()==qn)
					{
						q=temp;
					}
					Questions.read((char*)&temp, sizeof(temp));
				}
				displaydetails(q);
				cout<<endl<<"Press a key to return...";
				getch();
				Questions.close();
				return ;
			   case 3:
					Questions.close();
					return ;
			}
}

void reviewmistakes()
{
	fstream file;
	clrscr();
	int i=1,ch;
	cout<<"You have chosen REVIEW MISTAKES\n\n";
	Questions.open("CHEM_Q.dat", ios::in | ios::binary);

	//(below)select the questions with mistakes

	int total=0;
	QUESTION q, mis[50];
	Questions.read((char*)&q, sizeof(q));
	while(!Questions.eof())
	{
		if(q.return_mistakes()!=0)
		{
			mis[total]=q;
			total++;
		}
		Questions.read((char*)&q, sizeof(q));
	}
	Questions.close();
	if(total==0)
	{
		cout<<"You have made no mistakes!";
		cout<<"\n\nPress a key to return...";
		getch();
		return ;
	}
	//(below) questions get sorted in descending order of no. of mistakes done on it

	for(int x=0;x<total;++x)
	{
		for(int y=0;y<total-1-x;++y)
		{
			if(mis[y].return_mistakes()<=mis[y+1].return_mistakes())
			{
				    q=mis[y];
				    mis[y]=mis[y+1];
				    mis[y+1]=q;
			}
		}
	}

	//now display all the questions with mistakes
	int a=0;
	while(a<total)
	{
		clrscr();
		for(i=0;i<2;i++)
		{
			displaydetails(mis[a]);
			cout<<endl<<endl;
			a++;
			if(a>=total)
				break;
		}
		if(a<total)
		{
			cout<<"1. Next Page\n";
			cout<<"2. Return to menu\n";
			cout<<"Enter your choice: ";
			cin>>ch;
			while(!cin || ch<1 || ch>2)
			{
				cin.clear();
				cin.ignore();
				cout<<"\nInvalid choice!"<<endl;
				cout<<"1. Next Page\n";
				cout<<"2. Return to menu\n";
				cout<<"Enter your choice: ";
				cin>>ch;
			}
			if(ch==2)
			{
				return ;
			}
		}
		else
		{
			cout<<"All questions displayed...\n";
			cout<<"Press a key to return to menu";
			getch();
			return ;
		}
	}
}


int get_totalquestions()
{
	fstream file;
	file.open("CHEM_Q.dat", ios::in | ios::binary);
	if(!file)
	{
		notffile();
	}
	QUESTION Q;
	int N=0;
	file.read((char*)&Q, sizeof(Q));
	while(!file.eof())  //reads every object in file, increments N
	{
		N++;
		file.read((char*)&Q, sizeof(Q));
	}
	file.close();
	return N; //now N is equal to total number of questions (i.e. total objects)
}


void addquestion()
{
	UDDT chapter,type;
	int N, ch;
	QUESTION new_q, q;
	fstream ffilet, ffilec;
	
	add_choosechapter:
	ffilec.open("CHEMCHAP.dat", ios::binary | ios::in);
	if(!ffilec)
	{
		notffile();
	}

	select.chosenchapter.clear_data();
	chapter.clear_data();
	clrscr();
	cout<<"Choose the name of the chapter you want to add a question in:\n";
	N=0;
	ffilec.read((char*)&chapter, sizeof(chapter));
	while(!ffilec.eof())
	{
		N++;
		cout<<chapter.return_id()+1<<". "<<chapter.return_name()<<"\n";
		ffilec.read((char*)&chapter, sizeof(chapter));
	}
	cout<<"(OR)\n";
	cout<<N+1<<". Go back\n";
	cout<<"Enter your choice: ";
	cin>>ch;
	while(!ch || ch<1 || ch>N+1)
	{
		cin.clear();
		cin.ignore();
		cout<<"\nInvalid choice!";
		cout<<"\nPress a key to try again...";
		getch();
		ffilec.close();
		goto add_choosechapter;
	}
	if(ch==N+1)
	{
		ffilec.close();
		settings();
	}
	ffilec.clear();
	ffilec.seekg(0, ios::beg);
	ffilec.read((char*)&chapter, sizeof(chapter));
	while(!ffilec.eof())
	{
		if((chapter.return_id()+1)==ch)
		{
			select.chosenchapter.set_id(chapter.return_id());
			select.chosenchapter.set_name(chapter.return_name());
		}
		ffilec.read((char*)&chapter, sizeof(chapter));
	}
	ffilec.close();

	add_choosetype:
	ffilet.open("CHEMTYPE.dat", ios::in | ios::binary);
	if(!ffilet)
	{
		notffile();
	}

	select.chosentype.clear_data();
	type.clear_data();
	clrscr();
	cout<<"Choose a type of question: \n";
	N=0;
	ffilet.read((char*)&type, sizeof(type));
	while(!ffilet.eof())
	{
		N++;
		cout<<type.return_id()+1<<". "<<type.return_name()<<"\n";
		ffilet.read((char*)&type, sizeof(type));
	}
	cout<<"(OR)\n";
	cout<<N+1<<". Go back\n";
	cout<<"Enter your choice: ";
	cin>>ch;
	while(!cin || ch<1 || ch>N+1)
	{
		cin.clear();
		cin.ignore();
		cout<<"\nInvalid choice!";
		cout<<"\nPress a key to try again...";
		getch();
		ffilet.close();
		goto add_choosetype;
	}
	if(ch==N+1)
	{
		ffilet.close();
		goto add_choosechapter;
	}
	ffilet.clear();
	ffilet.seekg(0, ios::beg);
	ffilet.read((char*)&type, sizeof(type));
	while(!ffilet.eof())
	{
		 if((type.return_id()+1)==ch)
		 {
			select.chosentype.set_id(type.return_id());
			select.chosentype.set_name(type.return_name());
		 }
		 ffilet.read((char*)&type, sizeof(type));
	}
	ffilet.close();

	Questions.open("CHEM_Q.dat", ios::binary | ios::app);
	new_q.set_mistakes(0);
	new_q.set_qno(get_totalquestions());  //qno is the very next qno of the last question
	new_q.chapter.set_id(select.chosenchapter.return_id());
	new_q.chapter.set_name(select.chosenchapter.return_name());
	new_q.type.set_id(select.chosentype.return_id());
	new_q.type.set_name(select.chosentype.return_name());
	char tempq[200];
	int mks;
	cout<<"\nEnter the question:\n";
	gets(tempq);
	new_q.set_question(tempq);
	cout<<"\n\nEnter marks for the question: ";
	cin>>mks;
	while(!mks || mks<=0) //marks cannot be less than or equal to zero
	{
		cin.clear();
		cin.ignore();
		cout<<"\n\nInvalid entry!";
		cout<<"\nEnter marks for the question: ";
		cin>>mks;
	}
	new_q.set_marks(mks);
	Questions.write((char*)&new_q, sizeof(new_q));
	cout<<"\n\n\Question added!";
	cout<<"\nAssigned qno: "<<new_q.return_qno();
	cout<<"\n\nReturning...";
	delay(3200);
	Questions.close();
}

void Delete()
{
	int checkpassword, tempqno;
	QUESTION q;
	UDDT subject;
	select.chosenchapter.clear_data();
	select.chosentype.clear_data();
	clrscr();
	Questions.open("CHEM_Q.dat", ios::in | ios::binary);
	if(!ffile)
	{
		notffile();
	}
	cout<<"\nEnter qno of question you wish to delete: ";
	cin>>tempqno;
	if(tempqno>=get_totalquestions() || tempqno<0 || !cin)
	{
		cin.clear();
		cin.ignore();
		Questions.close();
		cout<<"Question does not exist!";
		cout<<"\n Press a key to return...";
		getch();
		return ;
	}
	Questions.seekg(tempqno*sizeof(q), ios::beg);
	Questions.read((char*)&q, sizeof(q));
	clrscr();
	cout<<"Details for selected question: \n";
	displaydetails(q);
	Questions.clear();
	Questions.seekg(0, ios::beg);
	cout<<"\n\nAre you sure you want to delete this question?(y/n)";
	cout<<"\n(Warning: This action cannot be undone!";
	char choice;
	cout<<"\nEnter your choice: ";
	cin>>choice;
	while(choice!='y' && choice!='Y' && choice!='n' && choice!='N')
	{
		cin.clear();
		cin.ignore();
		cout<<"\nInvalid choice!";
		cout<<"\nDo you want to delete this question?(y/n): ";
		cin>>choice;
	}
	if (choice=='y' || choice=='Y')
	{
		checkpassword=Enterpassword();
		if(checkpassword==1) //if it is the correct password, delete it
		{
			delete_question(tempqno);
			cout<<"\n\nQuestion deleted!";
			cout<<"\nReturning...";
			delay(1500);
			Questions.close();
			return;
		}
		else
		{
			cout<<"\nWrong password!";
			cout<<"\nReturning...";
			delay(1500);
			Questions.close();
			return;
		}
	}
	if (choice=='n' || choice=='N')
	{
			cout<<"\nReturning...";
			delay(1500);
			Questions.close();
			return;
	}

	Questions.close();
}

void settings()
{
	int ch;
	settings:
	clrscr();
	cout<<"Would you like to\n";
	cout<<"1.Add a new question\n";
	cout<<"2.Delete a question(Password required)\n";
	cout<<"3.Go back\n";
	cout<<"Enter your choice: ";
	cin>>ch;
	while(ch>3 || ch<1 || !cin)
	{
		cin.clear();
		cin.ignore();
		cout<<"\nInvalid choice!";
		cout<<"\nPress a key to try again...";
		getch();
		goto settings;
	}
	switch(ch)
	{
		case 1:
			addquestion();
			break;
		case 2:
			Delete();
			break;
		case 3:
			return;
	}
}


void Exit()
{
	char ch;
	clrscr();
	cout<<"Are you sure you want to exit?(y/n)";
	cout<<"\nEnter your choice: ";
	cin>>ch;
	while(ch!='y' && ch!='Y' && ch!='n' && ch!='N')
	{
		cin.clear();
		cin.ignore();
		cout<<"\nInvalid choice!";
		cout<<"\Enter your choice(y/n): ";
		cin>>ch;
	}
	if(ch=='y'||ch=='Y')
	{
		cout<<"\n\nSee you soon!";
		delay(1200);
		exit(0);
	}
	else if(ch=='n'||ch=='N')
		return;
}

void WelcomeScreen()
{
	int i=0;
	char line1[]={"RETRIEVAL PRACTICE PRODUCES MORE LEARNING"};
	char line2[]={"THAN ELABORATIVE STUDYING WITH CONCEPT MAPPING"};
	char line3[]={"-KARPICKE AND BLUNT"};
	char line4[]={", SCIENCE 2011"};

	clrscr();
	cout<<"\n\n\n\n\n\n\n\n\n\n\t\t  ";
	for(i=0;line1[i]!='\0';i++)
	{
		cout<<line1[i];
		delay(55);
	}
	cout<<"\n\t\t";
	for(i=0;line2[i]!='\0';i++)
	{
		cout<<line2[i];
		delay(55);
	}
	delay(300);
	cout<<"\n\n\n\n\t\t      ";
	delay(300);
	for(i=0;line3[i]!='\0';i++)
	{
		cout<<line3[i];
		delay(55);
	}
	delay(550);
	for(i=0;line4[i]!='\0';i++)
	{
		cout<<line4[i];
		delay(55);
	}
	delay(2600);
}


void main()
{
	WelcomeScreen();
	int ch, i,N;
	menu:
	select.chosenchapter.clear_data();
	select.chosensubject.clear_data();
	select.chosentype.clear_data();
	clrscr();
	cout<<"\t\t\t\tCHEMISTRY REVISION\n\n";
	cout<<"Would you like to\n";
	cout<<"1. Practice\n";
	cout<<"2. Browse\n";
	cout<<"3. Review your Mistakes\n";
	cout<<"4. Settings\n";
	cout<<"5. Exit\n";
	cout<<"\nEnter your choice: ";
	cin>>ch;
	while(!cin || ch<=0 || ch>=6)
	{
		cin.clear();
		cin.ignore();
		cout<<"\nInvalid choice!";
		cout<<"\nPress a key to try again...";
		getch();
		goto menu;
	}
	switch(ch)
	{

		case 1:
			UDDT type, chapter;
			practice_choosemethod:

			//selecting methodofpractice
			clrscr();
			cout<<"You have chosen PRACTICE\n\n";
			cout<<"Would you like to\n";
			cout<<"1.Practice a specific chapter\n";
			cout<<"2. Practice a specific type of question\n";
			cout<<"(OR)\n";
			cout<<"3. Go back\n";
			cout<<"Enter your choice: ";
			cin>>ch;
			while(!cin || ch<1 || ch>3)
			{
				cin.clear();
				cin.ignore();
				cout<<"\nInvalid choice!";
				cout<<"\nPress a key to try again...";
				getch();
				ffile.close();
				goto practice_choosemethod;
			}
			if(ch==3)
			{
				break;
			}
			else if(ch==1)
			{
				practice_choosechapter:
				ffile.open("CHEMCHAP.dat", ios::in | ios::binary);
				if(!ffile)
				{
					notffile();
				}

				//choosing chapter

				select.chosenchapter.clear_data();
				chapter.clear_data();
				clrscr();
				cout<<"Choose a chapter:\n ";
				N=0;
				ffile.read((char*)&chapter, sizeof(chapter));
				while(!ffile.eof())                //the list of options is present in the file, which is displayed here
				{
					N++;
					cout<<chapter.return_id()+1<<". "<<chapter.return_name()<<"\n";
					ffile.read((char*)&chapter, sizeof(chapter));
				}
				cout<<"(OR)\n";
				cout<<N+1<<". Go back\n";
				cout<<"Enter your choice: ";
				cin>>ch;
				while(!cin || ch<1 || ch>N+1) //numeric validation and error handling
				{
					cin.clear();
					cin.ignore();
					cout<<"\nInvalid choice!";
					cout<<"\nPress a key to try again...";
					getch();
					ffile.close();
					goto practice_choosechapter;
				}
				if(ch==N+1)
				{
					ffile.close();
					goto practice_choosemethod;
				}
				ffile.clear();
				ffile.seekg(0, ios::beg);
				ffile.read((char*)&chapter, sizeof(chapter));
				while(!ffile.eof())	//here the chosen chapter is stored in 'select' object of UDDT class
				{
					if((chapter.return_id()+1)==ch)
					{
						select.chosenchapter.set_id(chapter.return_id());
						select.chosenchapter.set_name(chapter.return_name());
					}
					ffile.read((char*)&chapter, sizeof(chapter));
				}
				ffile.close();
			}
			else if(ch==2)
			{

				practice_choosetype:
				ffile.open("CHEMTYPE.dat", ios::in | ios::binary);
				if(!ffile)
				{
					notffile();
				}

				//choosing type of question

				select.chosentype.clear_data();
				type.clear_data();
				clrscr();
				cout<<"Choose a type of question: \n";
				N=0;
				ffile.read((char*)&type, sizeof(type));
				while(!ffile.eof())
				{
					N++;
					cout<<type.return_id()+1<<". "<<type.return_name()<<endl;
					ffile.read((char*)&type, sizeof(type));
				}
				cout<<"(OR)\n";
				cout<<N+1<<". Go back\n";
				cout<<"Enter your choice: ";
				cin>>ch;
				while(!cin || ch<1 || ch>N+1)
				{
					cin.clear();
					cin.ignore();
					cout<<"\nInvalid choice!";
					cout<<"\nPress a key to try again...";
					getch();
					ffile.close();
					goto practice_choosetype;
				}
				if(ch==N+1)
				{
					ffile.close();
					goto practice_choosemethod;
				}
				ffile.clear();
				ffile.seekg(0, ios::beg);
				ffile.read((char*)&type, sizeof(type));
				while(!ffile.eof())
				{
					if((type.return_id()+1)==ch) 	//here the chosen type of question is stored in 'select' object of class UDDT
					{
						select.chosentype.set_id(type.return_id());
						select.chosentype.set_name(type.return_name());
					}
					ffile.read((char*)&type, sizeof(type));
				}
				ffile.close();
			}
			clrscr();
			createpool();
			displayquestions();
			break;
		case 2:
			browse();
			break;
		case 3:
			reviewmistakes();
			break;
		case 4:
			settings();
			break;
		case 5:
			Exit();
			break;
	}
	goto menu;
}
