
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include "../cjson/cJSON.h"
// use namespace std;
/* Parse text to JSON, then render back to text, and print! */
void doit(char *text)
{
	char *out;cJSON *json;
	
	json=cJSON_Parse(text);
	if (!json) {printf("Error before: [%s]\n",cJSON_GetErrorPtr());}
	else
	{
		out=cJSON_Print(json);
		cJSON_Delete(json);
		printf("%s\n",out);
		free(out);
	}
}

/* Read a file, parse, render back, etc. */
void dofile(char *filename)
{
	FILE *f=fopen(filename,"rb");fseek(f,0,SEEK_END);long len=ftell(f);fseek(f,0,SEEK_SET);
	char *data=(char*)malloc(len+1);fread(data,1,len,f);fclose(f);
	doit(data);
	free(data);
}

/* Used by some code below as an example datatype. */
struct record {const char *precision;double lat,lon;const char *address,*city,*state,*zip,*country; };

/* Create a bunch of objects as demonstration. */
void create_objects()
{
	cJSON *root,*fmt,*img,*thm,*fld;char *out;int i;	/* declare a few. */

	/* Here we construct some JSON standards, from the JSON site. */
	
	/* Our "Video" datatype: */
	root=cJSON_CreateObject();	
	cJSON_AddItemToObject(root, "name", cJSON_CreateString("Jack (\"Bee\") Nimble"));
	cJSON_AddItemToObject(root, "format", fmt=cJSON_CreateObject());
	cJSON_AddStringToObject(fmt,"type",		"rect");
	cJSON_AddNumberToObject(fmt,"width",		1920);
	cJSON_AddNumberToObject(fmt,"height",		1080);
	cJSON_AddFalseToObject (fmt,"interlace");
	cJSON_AddNumberToObject(fmt,"frame rate",	24);
	
	out=cJSON_Print(root);	cJSON_Delete(root);	printf("%s\n",out);	free(out);	/* Print to text, Delete the cJSON, print it, release the string. */

	/* Our "days of the week" array: */
	const char *strings[7]={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
	root=cJSON_CreateStringArray(strings,7);

	out=cJSON_Print(root);	cJSON_Delete(root);	printf("%s\n",out);	free(out);

	/* Our matrix: */
	int numbers[3][3]={{0,-1,0},{1,0,0},{0,0,1}};
	root=cJSON_CreateArray();
	for (i=0;i<3;i++) cJSON_AddItemToArray(root,cJSON_CreateIntArray(numbers[i],3));

/*	cJSON_ReplaceItemInArray(root,1,cJSON_CreateString("Replacement")); */
	
	out=cJSON_Print(root);	cJSON_Delete(root);	printf("%s\n",out);	free(out);


	/* Our "gallery" item: */
	int ids[4]={116,943,234,38793};
	root=cJSON_CreateObject();
	cJSON_AddItemToObject(root, "Image", img=cJSON_CreateObject());
	cJSON_AddNumberToObject(img,"Width",800);
	cJSON_AddNumberToObject(img,"Height",600);
	cJSON_AddStringToObject(img,"Title","View from 15th Floor");
	cJSON_AddItemToObject(img, "Thumbnail", thm=cJSON_CreateObject());
	cJSON_AddStringToObject(thm, "Url", "http:/*www.example.com/image/481989943");
	cJSON_AddNumberToObject(thm,"Height",125);
	cJSON_AddStringToObject(thm,"Width","100");
	cJSON_AddItemToObject(img,"IDs", cJSON_CreateIntArray(ids,4));

	out=cJSON_Print(root);	cJSON_Delete(root);	printf("%s\n",out);	free(out);

	/* Our array of "records": */
	struct record fields[2]={
		{"zip",37.7668,-1.223959e+2,"","SAN FRANCISCO","CA","94107","US"},
		{"zip",37.371991,-1.22026e+2,"","SUNNYVALE","CA","94085","US"}};

	root=cJSON_CreateArray();
	for (i=0;i<2;i++)
	{
		cJSON_AddItemToArray(root,fld=cJSON_CreateObject());
		cJSON_AddStringToObject(fld, "precision", fields[i].precision);
		cJSON_AddNumberToObject(fld, "Latitude", fields[i].lat);
		cJSON_AddNumberToObject(fld, "Longitude", fields[i].lon);
		cJSON_AddStringToObject(fld, "Address", fields[i].address);
		cJSON_AddStringToObject(fld, "City", fields[i].city);
		cJSON_AddStringToObject(fld, "State", fields[i].state);
		cJSON_AddStringToObject(fld, "Zip", fields[i].zip);
		cJSON_AddStringToObject(fld, "Country", fields[i].country);
	}
	
/*	cJSON_ReplaceItemInObject(cJSON_GetArrayItem(root,1),"City",cJSON_CreateIntArray(ids,4)); */
	
	out=cJSON_Print(root);	cJSON_Delete(root);	printf("%s\n",out);	free(out);

}


void do1(char *text)
{
	char *out;cJSON *json;
	
	json=cJSON_Parse(text);
	if (!json) {printf("Error before: [%s]\n",cJSON_GetErrorPtr());}
	else
	{
		out=cJSON_Print(json);
		cJSON_Delete(json);
		printf("%s\n",out);
		free(out);
	}
}

void t1(){
	char text6[]="{\n\"name\": \"Jack\"\n}";	
	do1(text6);
}

class json{
	cJSON *j;
	char *text;
public:
	json(char *t){
		text = t;
	}
	int parse(){
		j =cJSON_Parse(text);
		if (!j) {
			printf("Error before: [%s]\n",cJSON_GetErrorPtr());
			return 0;
		}
		return 1;
	}
	char *get_string(char*key){
		return cJSON_GetObjectItem(j,key)->valuestring;
	}
	int get_int(char*key){
		cJSON *obj = cJSON_GetObjectItem(j,key);
		if (obj->type==cJSON_Number)
			return obj->valueint;
		else
			return -1;
	}
	double get_double(char*key){
		cJSON *obj = cJSON_GetObjectItem(j,key);
		if (obj->type==cJSON_Number)
			return obj->valuedouble;
		else
			return -1;
	}
	void complete(){
		cJSON_Delete(j);
	}
	~json(){}
};
class jack{
	json *j;
public:
	jack(char*filename){
		FILE *f=fopen(filename,"rb");fseek(f,0,SEEK_END);long len=ftell(f);fseek(f,0,SEEK_SET);
		char *text=(char*)malloc(len+1);fread(text,1,len,f);fclose(f);		
		j = new json(text);
		j->parse();
		free(text);
	}
	int get_age(){
		char age[] = "age";
		return j->get_int(age);
	}
	char*get_name(){
		char name[] = "name";
		return j->get_string(name);
	}
	~jack(){
		j->complete();
		delete j;
	}
};
class douban{
	json *j;
public:
	douban(char*filename){
		FILE *f=fopen(filename,"rb");fseek(f,0,SEEK_END);long len=ftell(f);fseek(f,0,SEEK_SET);
		char *text=(char*)malloc(len+1);
		memset(text,0,len+1);
		fread(text,1,len,f);fclose(f);	
		// printf("%s\n=--",text);	
		j = new json(text);
		j->parse();
		free(text);
	}
	~douban(){
		j->complete();
		delete j;
	}
	char* get_userid(){
		char age[] = "user_id";
		return j->get_string(age);
	}	
	char* get_err(){
		char age[] = "err";
		return j->get_string(age);
	}	
	char* get_token(){
		char age[] = "token";
		return j->get_string(age);
	}
	char* get_expire(){
		char age[] = "expire";
		return j->get_string(age);
	}
	double get_r(){
		char age[] = "r";
		return j->get_double(age);
	}
	int get_r_i(){
		char age[] = "r";
		return j->get_int(age);
	}
	char* get_username(){
		char age[] = "user_name";
		return j->get_string(age);
	}	
	char* get_email(){
		char age[] = "email";
		return j->get_string(age);
	}	
};
//{"user_id":"1830596","err":"ok","token":"588a4bce8c","expire":"1405751455","r":0,"user_name":"1000copy","email":"1000copy@gmail.com"}
int main (int argc, const char * argv[]) {	
	jack j ("t.json");
	printf("name :%s\n",j.get_name());
	printf("age  :%d\n",j.get_age());
	douban d ("login.json");
	printf("user_id:%s\n",d.get_userid());
	printf("get_err:%s\n",d.get_err());
	printf("get_token:%s\n",d.get_token());
	printf("get_r:%d\n",d.get_r_i());
	printf("get_username:%s\n",d.get_username());
	printf("get_email:%s\n",d.get_email());
	return 0;
}
