// 10727246 劉晉成 10727255 周子騰
#include <iostream>
#include <new> 
#include <string>
#include <vector>
#include <cstdlib> 
#include <cstring>
#include <fstream>
#include <time.h>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <stack>
#include <math.h>
#include <algorithm>
#include <queue>


using namespace std;
#define MAX_LEN 10              // array size of student id and name
#define PAGE     25              // amount of display on screen
#define NONE     -1              // error flag

class DirectedGraph
{   
    private:
	    typedef struct sP            // student pair
	    {  char    sid1[MAX_LEN];    // 1st sid: sender
	       char    sid2[MAX_LEN];    // 2nd sid: receiver
	       float   wgt;              // pair weight
	    }  studentPair;
	
	    typedef struct aLN           // node of adjacency lists
	    {   string    sid2;          // receiver
	        float     weight;        // pair weight
	        struct aLN  *next;       // pointer to the next node
	    }   adjListNode;
	
	    typedef struct aL            // adjacency list
	    {  string   sid1;            // sender
	       adjListNode *head;        // pointer to the first node of a list
	       int        inf;           // infiuence value
	       vector<string>  visited;   //  經過的學號 
	    }  adjList;
	    
	    vector<adjList> adjL;        // the adjacency lists
	    string      fileNo;          // a number to form a file name
	    float       wgtLB;           // lower bound of weights
	    int         stNo;            // file size
	    vector<string>  stnum ;
	    
	    bool readF(vector<studentPair> & student) {                  // get all records from a file
            fstream      binFile;                    // input file handle
            studentPair  oneSt;                      // keep one record with the required fields
            stNo = 0;                   // total number of students
            string filename;
            filename = "pairs" + fileNo + ".bin";
            
            binFile.open(filename.c_str(), fstream::in | fstream::binary) ;  //  open a binary file
            if(binFile.is_open())
            {   binFile.seekg(0, binFile.end) ;
                stNo = binFile.tellg() / sizeof(oneSt) ;
                binFile.seekg(0, binFile.beg) ;
		
	            for (int i = 0 ; i < stNo ; i++)
	            {   binFile.read((char*)&oneSt, sizeof(oneSt));  //  read the entire record from from a binary file
	                student.push_back( oneSt ) ;
	            
	            } // end for 
	    
	        }  //  end if
	        binFile.close();                     //  close input file 
	        
	        return true;
        }
    
    
    
	    void insert(adjList & aj ) 
	    {    adjL.push_back( aj ) ;
	    	
		}
	    
	    int locate(vector<adjList> adjL, string key)            // locate the index in adjacency lists
	    {   int num = 0,i = 0;
		    while( i < adjL.size() ) {
	    	    if( adjL[i].sid1.compare( key ) == 0 ) {
	    	    	num = i ;
	    	    	break;
				}
				
				i++;   
		    }
		    
		    return num;
		}
	    int locate(string & key) { return locate(adjL, key); }  // locate the index in adjacency lists
	    bool addCount(adjListNode * a, adjListNode * b);            // count only if not visited yet
	    void saveINF(vector<adjList> & a, string b);                // wirte infiuence values as a file
	    bool haveSameIndex( vector<string> array, string key ) 
		{   for ( int i = 0 ; i < array.size() ; i++ ) {
			    if ( array[i].compare( key) == 0) 
			    	return true;
				
		    } 
		    
		    return false;
		}
		
		
	
	    
    public:
    	
        
    	void setfile( string filename )         // 設定檔案名 
	    {    fileNo = filename;
		}
        DirectedGraph(): fileNo(""), wgtLB(0) {}               // default constructor
        DirectedGraph(DirectedGraph & obj): adjL(obj.adjL), fileNo(obj.fileNo), wgtLB(0)
        {   }                                                  // shallow copy constructor
	    bool existed(){     return adjL.size(); }              // check the existence
	    void setLB(float v) { wgtLB = v; }                     // set up the value of wgtLB
	   
	    bool create() {                                         // read pairs from a file into adjacency lists
            vector<studentPair>  student;
	        adjList  aj ;
	        adjListNode *adjNo; 
	        int j = 0;
	        if( !readF( student ) )
	            return false;
	        
	        for( int i = 0 ; i < stNo ; i++ ) {          //  尋找放的位置        
		        adjNo = new adjListNode() ;
	            adjNo->sid2 = student[i].sid2;
	            adjNo->next = NULL;
	            adjNo->weight = student[i].wgt;
	            aj.head = adjNo;
	            aj.sid1 = student[i].sid1 ;
	            if ( adjL.size() == 0 ) {
		            insert( aj ) ;
	            }
	            else {
	    	        for( j = 0 ; j < adjL.size() ; j++ ) {
	    		        if( adjL[j].sid1 == aj.sid1  ) {
	    			        adjListNode *temp = adjL[j].head;
	    		            for( adjListNode *walk = temp->next ; temp != NULL ; temp = temp->next, walk=walk->next ) {      
	    		    	        if( walk == NULL ) {      //  只有一個   
	    		    		        if( temp->sid2 < adjNo->sid2 ) {
	    		    			        temp->next = adjNo;
							        }
							        else {
								        adjNo->next = temp;
								        temp = adjNo;
								        adjL[j].head = temp;
							        }
							
							        break;
						        }
						        else if( walk->next == NULL ) {      // 2個 
						            if( temp->sid2 > adjNo->sid2 ) {
	    		    			        adjNo->next = temp;
								        temp = adjNo;
								        adjL[j].head = temp;
							        }
							        else if ( walk->sid2 > adjNo->sid2 ) {
							  	        adjNo->next = walk;
								        temp->next = adjNo;
							        }
							        else {
								        walk->next = adjNo;
							        }
							
							        break;
						        }
						        else {           // 多個 
							        if ( walk->sid2 > adjNo->sid2 ) {
								        adjNo->next = walk;
								        temp->next = adjNo;
								        break;
							        }		
						        }
					        }  //  end  for	
					
					        break;
				        }  /// end if
			        }  // end  for
			
			        if( j == adjL.size() )      // 沒有找到相同的 sender
			            insert( aj ) ;
			    
			    
			    
			    
		        }
            }  //  end for
    
            for( int i = 0 ; i < adjL.size() ; i++ ) {        // 都沒有當過sender  
    	        for( adjListNode *temp = adjL[i].head ; temp != NULL ; temp=temp->next ) {
    		        for( j = 0; j < adjL.size() ; j++ ) {
    			        if( temp->sid2 == adjL[j].sid1 ) 
    			            break;
    			
		            }
		    
		            if ( j == adjL.size() ) {    // 沒當過sender 
		                adjList  oneL ;
				        oneL.sid1 = temp->sid2 ;
				        oneL.head = NULL;
				        insert( oneL );	
			        }
		        }
	        }
    
            sort( adjL.begin(), adjL.end(), SortObj ) ;  // 排序 
    
	        return true;

        }
	    
	    
	    
        
        
        
        
		void saveF()                                           // write adjacent lists as a file
		{   int num = 0, j = 1;
	        string filename;
	        filename = "pairs" + fileNo + ".adj";
	        fstream file;
	        file.open(filename.c_str(), ios::out );
	        if ( file.is_open() ) {
	            file << "<<< There are " << adjL.size() << " IDs in total. >>>" << endl;
                for ( int i = 0 ; i < adjL.size(); i++ ) {
      	            file << "[" << i+1 << "]" << adjL[i].sid1 << ":" << endl ;
      	            j=1;
		            for( adjListNode *walk = adjL[i].head ; walk != NULL ; walk = walk->next ){
		  	            file << "\t(" << j << ")\t" << walk->sid2 << ", " << walk->weight ;
		            
		                if ( j%10 == 0) 
		  	                file << endl;
		                j++;
		                num++;
                    }
                    
                    file << endl;
                }
            }
    
    
            file << "<<< There are " << num << " nodes in total. >>>" << endl;
	        file.close() ;
	        
	        cout << "<<< There are " << adjL.size() << " IDs in total. >>>" << endl << endl;
	        cout << "<<< There are " << num << " nodes in total. >>>" << endl << endl;
			
		}
		
		void saveF2()                                           // write adjacent lists as a file
		{   int num = 0, j = 1;
	        string filename;
	        filename = "pairs" + fileNo + ".cnt";
	        fstream file;
	        file.open(filename.c_str(), ios::out );
	        if ( file.is_open() ) {
	            file << "<<< There are " << adjL.size() << " IDs in total. >>>" << endl;
                for ( int i = 0 ; i < adjL.size(); i++ ) {
      	            file << "[" << i+1 << "]" << adjL[i].sid1 << "(" << adjL[i].inf << ")"<< ":" << endl ;
		            for(  j = 1 ; j <= adjL[i].visited.size() ; j++ ) {
		  	            file << "\t(" << j << ")\t" << adjL[i].visited[j-1] ;
		            
		                if ( j%10 == 0) 
		  	                file << endl;
		                
                    }
                    
                    file << endl;
                }
            }
    

	        file.close() ;
	        
	        cout << "<<< There are " << adjL.size() << " IDs in total. >>>" << endl << endl;
	        
			
		}
		
	    void compINF()                                   // compute influence values by BFS 
	    {    queue<string> que;
	         int index;
		     
	         for( int i = 0 ; i < adjL.size() ; i++ ) {
	         	que.push( adjL[i].sid1 ) ;
	         	
	         	while( !que.empty() ) {               
	         		index = locate( que.front() ) ;    //  找出所在的地方 
	         	    for( adjListNode *temp = adjL[index].head ; temp != NULL ; temp = temp->next ) {
	         	        if( !haveSameIndex( adjL[i].visited, temp->sid2 ) && adjL[i].sid1.compare( temp->sid2 ) != 0 ) {
	         	            adjL[i].visited.push_back( temp->sid2 );
							que.push( temp->sid2 ) ; 
						}	
						
						
	         	    	
					}
					que.pop();   
					
			    }
			    
			   
       	        sort( adjL[i].visited.begin(), adjL[i].visited.end(), SortVisit ) ;         
       	        
			    adjL[i].inf = adjL[i].visited.size() ;  
	         }
	         
	         sort( adjL.begin(), adjL.end(), Sortinf ) ;       // 排序inf大小 
	        
		}
		
		struct SortObj {
	        bool operator() ( adjList a, adjList b ) {
		        return( a.sid1 < b.sid1 ) ;
	        }
        } SortObj;	
        
	    struct Sortinf {
	        bool operator() ( adjList a, adjList b ) {
	        	if( a.inf > b.inf ) 
				    return true ;
				else if( a.inf == b.inf && a.sid1 < b.sid1 )    
				    return true;
				else return false;    
	        }
        } Sortinf;	
        
        struct SortVisit {
	        bool operator() ( string a,  string b ) {
		        return( a < b ) ;
	        }
        } SortVisit;	
	    
	    void clearUp( ) {                      // 清除資料 
	    	adjL.clear();
		}
	    
	    ~DirectedGraph()  { clearUp(); }                 // destructor: destroy the object
 	
	   
	   	    
}; // class DirectedGraph












int  main()
{   int command = 0 ;
	string fileName = "" ;
	
	DirectedGraph *compare = new DirectedGraph();
	bool one = false;

	do {
		cout << "**** Graph data manipulation ****" << endl;
		cout << "* 0. QUIT                       *" << endl;
		cout << "* 1. Build adjacency lists      *" << endl;
		cout << "* 2. compute connection counts  *" << endl;
		cout << "**********************************************" << endl;
		cout << "Input a choice(0, 1, 2): " ; 
		cin >> command ;
		cout << endl  ;
		if ( command == 0 )
			break ;
		else if (command == 1 ) {
			cout << "Input a file number ([0] Quit): " ;
			cin >> fileName ;
			cout << endl  ;
			compare->setfile( fileName ) ;
			if ( one == true ) {
				compare->clearUp();
                one = false;
			}
			
			if ( !compare->existed() ) {
				compare->create();
			    compare->saveF();
				one = true;
		    }
				 	 
			    
		    
			
				
			
			
		
			
			
			
		
		}
		else if ( command == 2 ) {
			if ( compare->existed() ) {
				compare->compINF();
			    compare->saveF2();
			}
			else {
				cout << "### There is no graph and choose 1 first. ###" << endl;
				
		    }
			
		    
			
		
		}	
		else {
			cout << "Command dose not exist! " << endl << endl ; 		
			
		}
		
	} while ( command != 0 ) ;

    


    
  
    system("pause") ;
    
    
    return 0 ;
}   //  end  main


