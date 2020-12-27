#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

//#include<graphics.h>
int auth=0,i,j;
char user[100];
const int rooms = 25;
struct guest
{
    int room;
    int avail;
    char name[30];
    char contact[20];

};
struct food
{
    char item[50];
    float rate ;   
};
typedef struct food food;
struct bill
{
    int room;
    int ordered;
    food room_service[20];
    int rent_per_day;
    int days;
    float rent;
    float total;

};
typedef struct bill bill;


typedef struct guest guest;
void green(int x)
{
	printf("\x1b[32m%d\x1b[0m",x);

}
void red(int x)
{
	printf("\x1b[31m%d\x1b[0m",x);
}
void gwrite(guest *g)
{

    FILE *f;
    f=fopen("guest.bin","w");
    for(int i=0;i<rooms;++i)
    {
        fwrite(&g[i],sizeof(guest),1,f);
        //printf("writing %d to room %d",i+1,g[i].room);
    }
    fclose(f);
}

void gread(guest *g)
{
    FILE *fp;
    fp=fopen("guest.bin","r");

   // fseek(f,sizeof(guest)*20,0);
    for(int i=0;i<rooms;++i)
    {
        fread(&g[i],sizeof(guest),1,fp);
        //printf("reading %d room %d\n",i+1,g[i].room);
    }
    fclose(fp);
}

void bill_constructor ( )
{   bill b[rooms];
    FILE *fp=fopen("bill.bin","w");
   
    for (int i = 0; i < rooms; ++i)
    {
         b[i].ordered=0;
        b[i].room=i+1;
        for (int j = 0; j < 20; ++j)
        {
            strcpy(b[i].room_service[j].item,"nothing");
            b[i].room_service[j].rate=0;
        }
        if(b[i].room<11)    b[i].rent_per_day=500;
        else if(b[i].room<21)   b[i].rent_per_day=750;
        else if (b[i].room<26) b[i].rent_per_day=1000;
        b[i].rent=0;
        b[i].days=0;

        b[i].total=0;
    }
    fwrite(b,sizeof(bill),rooms,fp);
    fclose(fp);

}

void bill_generate(int room)
{
	  system("clear");
    system("clear");
    FILE *f;

    f=fopen("bill.bin","r");
    bill b[rooms]; 
    fread(b,sizeof(bill),rooms,f);
    fclose(f);
    guest g[rooms];
    gread(g);
                    
    if(g[room-1].avail==1)   
    {
      printf("Room is not Occupied\n");
       
      printf("Press Enter to continue...");
    	getchar();getchar();
    	return; 
    }                   
    
    bill new=b[room-1];
     
    int days;
    printf("Enter number of days stayed : ");
    scanf("%d",&days);
    new.days=days;
    new.rent=new.days*new.rent_per_day;
    new.total+=new.rent;

    printf("----------------------------------------------------\n" );
    printf("\t\t\tBILL\n");
    printf("----------------------------------------------------\n" );
    printf("%s %18d\n","Room Number ",new.room);
    printf("%s %13s\n","Name of the guest",g[room-1].name);
    printf("%s %15s\n","Contact number ",g[room-1].contact );
    printf("%s %15d\n","Rent per day is",new.rent_per_day);
    printf("%s %8d\n","Number of days stayed ",new.days);
    

   
    printf("\nRoom service orders\n");
    printf("-------------------------------------\n" );
    printf("%15s\t%17s\n","Item ordered","Cost");
    printf("-------------------------------------\n" );
    float total;
    for (int i = 0; i < new.ordered; ++i)
    {
        food f=new.room_service[i];
        printf("%15s\t%18.2f\n",f.item,f.rate);
        total+=f.rate;
        
    }
    printf("-------------------------------------\n" );
    printf("%s %14.2f\n","Room service total ",total);
    printf("%s %14.2f\n","Total room rent  is",new.rent);
    new.total+=total;
    printf("\n----------------------------------------------------\n" );
    printf("Total bill is %20.2f\n",new.total);
    printf("----------------------------------------------------\n" );
    
    
    printf("Press Enter to continue...");
    getchar();
    getchar();

}

void def()
{
    guest g[rooms];
  

    FILE *f; FILE *fp;
    f=fopen("guest.bin","w");
    
    for(int i=0;i<rooms;++i)
    {
        g[i].room=i+1;
        strcpy(g[i].contact,"initialized");
        strcpy(g[i].name,"initialized");
        g[i].avail=1;
       


        printf("writing %d to room %d\n",i+1,g[i].room);
        fwrite(&g[i],sizeof(guest),1,f);
    }
    fclose(f);

}


void login()
{
	int c;
    log:
	printf("Welcome to XYZ hotel\n");
	printf("1. Guest login\n");
	printf("2. Admin login\n");
    printf("3. Exit\n");
    printf("Enter choice : ");
	scanf("%d",&c);
	if(c==1)   
    {   auth=0;
        strcpy(user,"Guest");

    }
    if(c==2)
	{
        char a[100];
        printf("Enter password: ");
        
        char *b=getpass(a);
        if(strcmp(b,"abc")==0) 
        {   auth=1;
            strcpy(user,"Admin");
        }
        else
        {
            printf("Invalid password\n");
            goto log;
        }

	}
    if(c==3)exit(0); 

}

int menu()
{
	int choice;

	printf("Choose from the list of options below\n");
	;
	printf("1. Check in\n");
	printf("2. Room Service\n");
	printf("3. Check out\n");
    printf("4. Current Bill info\n");
    printf("5. Check room availability\n");
	printf("6. Exit\n");
	printf("Your choice : ");
	scanf("%d",&choice);
	return choice;
}

void check_in()
{
    int n,p,room;
    guest g[rooms],new_guest;
    system("clear");
    system("clear");

  printf("Enter your name : ");
    scanf(" ");
    gets(new_guest.name);
    printf("Enter your phone number : ");
    scanf(" ");
    gets(new_guest.contact);
    
    /*strcpy(new_guest.name,"new");
    strcpy(new_guest.contact,"new");
*/
    gread(g);
    printf("Checking availability of rooms....\n");
    printf("========================================\n");
    for(int i=0;i<rooms;++i)
    {


        /*printf("Room number: %d\t",(g[i].room));
        if(g[i].avail)
            printf("Available");
        else
            printf("Occupied");
        if(i%2) printf("\n");
        else printf("\t");
        */

		

        if(g[i].avail)
            green(g[i].room);
        else
            red(g[i].room);
        if((i+1)%5) printf("\t");
        else printf("\n");
        


    }

    printf("========================================\n");
    printf("");
    printf("Rooms 1-10 are Single (Rs500/day)\n");
    printf("Rooms 11-20 are Double (Rs750/day)\n");
    printf("Rooms 21-25 are Quad (Rs1000/day)\n");
    /*
    printf("Enter number of people : ");
    scanf("%d",&n);
    printf("Enter your package (1 for basic & 2 for deluxe) : ");
    scanf("%d",&p);
    */
    num:
    printf("Enter the room number: ");
    scanf("%d",&room);

    if(room>rooms||room<1)
    {
        printf("Invalid room number\n");
        goto num;
    }
    if(g[room-1].avail!=1)
    {
        printf("Already filled\n");
        goto num;
    }
    printf("Checking in");
    new_guest.room=room;

    new_guest.avail=0;
    //strcpy(new_guest.contact,"def");
    //strcpy(new_guest.name,"def");
    g[room-1]=new_guest;
    gwrite(g);
    
   

    system("clear");

   // gread();3;
}

void check_out(int r)
{  
    bill_generate(r);
    
    
    guest g[rooms];
    gread(g);
    g[r-1].avail=1;
    gwrite(g);
}

void  service(int r)
{	
	guest g[rooms];
    gread(g);
                    
    if(g[r-1].avail==1)   
    {
      printf("Room is not Occupied\n");
       
      printf("Press Enter to continue...");
    	getchar();getchar();
    	return; 
    }  

    ser:   
	system("clear");
   	system("clear");
    FILE *f; bill b; int c;
    char items[10][25]={"Tea","Coffee","Idly","Poori","Pongal","Dosa",
    					"Meals","Briyani","Chappathi","Parrotta"};
    int rate[10]={10,10,15,20,30,40,60,100,20,20};
    f=fopen("bill.bin","r+");
    fseek(f,sizeof(bill)*(r-1),0);
    fread(&b,sizeof(bill),1,f);
    printf("------------------------------------------\n" );
    printf("%7s%15s\t%13s\n","Choice","Item","Rate");
    printf("------------------------------------------\n" );
    for (int i = 0; i < 10; ++i)
    {
    	printf("%7d%15s\t%10s%3d\n",i+1,items[i],"Rs. ",rate[i]);
    }
    printf("------------------------------------------\n" );
    
 
    ch:
    printf("Enter your choice : ");
    /*char str[100];
    gets(str);
    int choices[10][10],x=0,y=0; 
    for (int i = 0; str[i]; ++i)
    {
    	if(str[i]==' ')
    		x++;
    	else
    		choices[x][y++]=str[i];
    }
    for (int i = 0; i <=x; ++i)
    {
    	c=atoi(choices[i]);
    */
    scanf("%d",&c);
    if(c>10 || c<1)
    {
        printf("Invalid choice %d\n",c);
        goto ch;
    }
    int i=b.ordered;
    strcpy(b.room_service[i].item,items[c-1]);
    b.room_service[i].rate=rate[c-1];
    b.ordered++;
    fseek(f,sizeof(bill)*(r-1),0);
    fwrite(&b,sizeof(bill),1,f);
    fclose(f);
	
    printf("Do you want to order more (y/n) :");
    getchar();
    char a=getchar();
    if(a=='y')	goto ser;
    else ;
}
int authenticate()
{
    if(auth==0)
    {
        printf("Your do not access to perform this task\n");
        return 0;
    }
    else return 1;

}
int main()
{
 // def();
////  gread(g);
    start:
    system("clear");
        system("clear");
    printf("---------------------------------------------------------------\n");
        printf("Hotel Management Software\n");
        printf("---------------------------------------------------------------\n");
    login();
	while(1)
	{
        int r;
        system("clear");
   		system("clear");
        printf("---------------------------------------------------------------\n");
        printf("Hotel Management Software\t");printf("User : %s\n",user);
        printf("---------------------------------------------------------------\n");
        
		switch(menu())
		{
			
			case 1:
			{
                    check_in(); bill_constructor();
                   
                    break;
            }
            case 6: goto start;
            case 2: if(!authenticate())   
                    {printf("Press Enter to continue...");
                    getchar();
                    getchar();   continue;
                    }  
                    printf("Enter room number : ");
                    scanf("%d",&r);
                    service(r);break;
            case 3: if(!authenticate())   
                    {printf("Press Enter to continue...");
                    getchar();
                    getchar();   continue;
                    } 
                     printf("Enter room number : ");
                     scanf("%d",&r);
                    check_out(r);break;
            case 4: 
                    printf("Enter room number : ");
                     scanf("%d",&r);

                     bill_generate(r);
                    break;
            case  5: 
        		    system("clear");
   					system("clear");guest g[rooms]; gread(g);
					printf("Checking availability of rooms....\n");
				    printf("========================================\n");
				    for(int i=0;i<rooms;++i)
				    {

				        if(g[i].avail)
				            green(g[i].room);
				        else
				            red(g[i].room);
				        if((i+1)%5) printf("\t");
				        else printf("\n");

				    }
				    printf("Press Enter to continue...");
				    getchar();
				    getchar();
				    break;



           // case 8: exit(0);break;
		}

	}

return 0;
}
