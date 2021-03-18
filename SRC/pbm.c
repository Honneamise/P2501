#include "pbm.h"


/**********/
int FileBuffer(char *file, unsigned char **buffer)
{
	int size = 0;

	unsigned char *buf = NULL;

	FILE *fp = fopen(file, "rb");

	if (fp != NULL)
	{
		if (fseek(fp, 0, SEEK_END) != 0) { printf("[%s]: Error seeking file \"%s\"\n", __func__, file); exit(EXIT_FAILURE);};

		size = (int)ftell(fp);

		buf = calloc( size, sizeof(unsigned char));

		if(buf==NULL) { printf("[%s]: allocation error", __func__); exit(EXIT_FAILURE); }

		if (fseek(fp, 0, SEEK_SET) != 0) { printf("[%s]: Error seeking file \"%s\"\n", __func__, file); exit(EXIT_FAILURE);};

		size_t check = fread(buf, size, 1, fp);

		if (check != 1) { printf("[%s]: Error reading file \"%s\"\n", __func__, file); exit(EXIT_FAILURE);};

		fclose(fp);
	}

	else
	{
		printf("[%s]: Unable to open file \"%s\"\n", __func__, file);
	}

	*buffer = buf;

	return size;
}

/**********/
char *SkipSpaces(char *p)
{
	if(p==NULL){ return NULL; }

	while( (*p!=0) && ( *p==' ' || *p=='\t'))
	{
		p++;
	}

	if(*p==0){ return NULL; }

	return p;
}

/**********/
char *SkipBlanks(char *p)
{
	if(p==NULL){ return NULL; }

	while( (*p!=0) && ( *p==' ' || *p=='\t' || *p=='\r' || *p=='\n' ))
	{
		p++;
	}

	if(*p==0){ return NULL; }

	return p;
}

/**********/
char *SkipLine(char *p)
{
	if(p==NULL){ return NULL; }
	
	while(*p!=0 && *p!='\n')
	{
		p++;
	}

	if(*p==0){ return NULL; }

	p++;

	return p;
}

/**********/
char *ReadUntil(char *p, char symbol)
{
	if(p==NULL){ return NULL; }
	
	while((*p!=0) && (*p!=symbol))
	{
		p++;
	}

	return p;
}

/**********/
char *ReadUntilBlank(char *p)
{
	if(p==NULL){ return NULL; }
	
	while((*p!=0) && ( *p!=' ' && *p!='\t' && *p!='\r' && *p!='\n' ))
	{
		p++;
	}

	return p;
}

/**********/
int IsNumber(char *p)
{
	int len = (int)strlen(p);

	int res = 0;

	for(int i=0;i<len;i++)
	{
		if(isdigit(p[i])) { res = 1; }
		else { res=0; break;}
	}

	return res;
}

/**********/
void PbmLoad(char *file, unsigned char **buffer, int *w, int *h)
{
	int _w = 0;
	int _h = 0;

	char *token = NULL;
	unsigned char *buf = NULL;

	int size = FileBuffer(file, &buf);

	//read magic
	unsigned char *start = buf;
	unsigned char *end = ReadUntilBlank(start);
	if(end==NULL){ printf("[%s]: error parsing file \"%s\"\n", __func__, file); exit(EXIT_FAILURE); }

	token = calloc(end-start+1,sizeof(char));
	if(token==NULL) { printf("[%s]: allocation error", __func__); exit(EXIT_FAILURE); }

	//strncpy(token, start, end-start);
	memcpy(token, start, end-start);

	if(strcmp(token, PBM_MAGIC)!=0){ printf("[%s]: invalid file format\n", __func__); exit(EXIT_FAILURE);}

	free(token);

	//read comment if present
	start = SkipBlanks(end);
	if(start==NULL){ printf("[%s]: error parsing file \"%s\"", __func__, file); exit(EXIT_FAILURE);}

	if(start[0]=='#'){ start = SkipLine(start); };

	//read width
	end = ReadUntilBlank(start);
	if(end==NULL){ printf("[%s]: error parsing file \"%s\"", __func__, file); exit(EXIT_FAILURE);}

	token = calloc(end-start+1,sizeof(char));
	if(token==NULL) { printf("[%s]: allocation error", __func__); exit(EXIT_FAILURE); }

	strncpy(token, start, end-start);

	if(!IsNumber(token)){ printf("[%s]: error parsing file \"%s\"", __func__, file); exit(EXIT_FAILURE);}

	_w = atoi(token);

	free(token);

	//read height
	start = SkipBlanks(end);
	if(start==NULL){ printf("[%s]: error parsing file \"%s\"", __func__, file); exit(EXIT_FAILURE);}

	end = ReadUntilBlank(start);
	if(end==NULL){ printf("[%s]: error parsing file \"%s\"", __func__, file); exit(EXIT_FAILURE);}

	token = calloc(end-start+1,sizeof(char));
	if(token==NULL) { printf("[%s]: allocation error", __func__); exit(EXIT_FAILURE); }

	strncpy(token, start, end-start);
	
	if(!IsNumber(token)){ printf("[%s]: error parsing file \"%s\"", __func__, file); exit(EXIT_FAILURE);}

	_h = atoi(token);

	free(token);
	
	//pointer to image data
	start = SkipLine(end);

	*buffer = calloc(_w*_h/8,sizeof(unsigned char));
	if(*buffer==NULL) { printf("[%s]: allocation error", __func__); exit(EXIT_FAILURE); }

	for(int i=0;i<_w*_h/8;i++)
	{
		(unsigned char)(*buffer)[i] = ~start[i];
	}

	free(buf);

	*w = _w;
	*h = _h;
}