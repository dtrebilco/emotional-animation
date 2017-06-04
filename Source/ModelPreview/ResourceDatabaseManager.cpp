/*--------------------------------------------------------------------
  ResourceDatabaseManager.cpp

  Contains the definition a class used to manage the creation and 
  allocation of resource databases. (Came about as you cannot have a 
  resource database pointing to the same directory and then delete both)

  21/03/2001 -- Created the file

By Damian Trebilco
   Copyright © 2001. All Rights Reserved.
--------------------------------------------------------------------*/
#include "stdafx.h"
#include "ResourceDatabaseManager.h"

using namespace Jet;

LOGCLIENT(ResourceDatabaseManager);


//Init. Static variables
unsigned int ResourceDatabaseManager::numLoadingRDB=0;
ResourceDatabaseManager::DBArray      ResourceDatabaseManager::databases;
ResourceDatabaseManager::DBNameArray  ResourceDatabaseManager::names;
ResourceDatabaseManager::DBPathArray  ResourceDatabaseManager::paths;
ResourceDatabaseManager::DBReferences ResourceDatabaseManager::references;

/*-------------------------------------------------------------------
PROCEDURE: ResourceDatabaseManager
PURPOSE  : To construct the object
PRE : TRUE
POST: The object has been constructed.
-------------------------------------------------------------------*/
ResourceDatabaseManager::ResourceDatabaseManager(){
  //Increase the reference count for the number of times this class 
  //  is loaded
  numLoadingRDB++;
}


/*-------------------------------------------------------------------
PROCEDURE: ~ResourceDatabaseManager
PURPOSE  : To destroy the object
PRE : TRUE
POST: The object has been destroyed.
-------------------------------------------------------------------*/
ResourceDatabaseManager::~ResourceDatabaseManager(){
int numDB;

   //Decrease number of class instances
   numLoadingRDB--;

   numDB=databases.Count();

   //Warn that there are databases outstanding
   if(numDB>0 && numLoadingRDB==0){
      LOGERR(("ResourcesDatabases: %d databases left unloaded",numDB));

      //Cannot delete remaining DB's as the Jet system may be deleting them...
      //DestroyAllDatabases();
   }
}


/*-------------------------------------------------------------------
PROCEDURE: LoadDatabase
PURPOSE  : To load a database from a given full path
PRE : TRUE
POST: Returns a ResourceDatabase that is a best guess of the 
      path.
-------------------------------------------------------------------*/
ResourceDatabase * ResourceDatabaseManager::LoadDatabase(PString path){
int i;
int len;
int offset=0;
  
  len=path.Length();

  //Get the optional last character 
  if(path[len-1]=='/'||path[len-1]=='\\'){
    offset=1;
  }

  //Search the string for a "/" 
  for(i=len-1-offset;i>=0;i--){
    if(path[i]=='/'||path[i]=='\\'){
        break;
    }
  }

  //If we found a "/"
  if(i>-1){
    //Split the string
    PString pLeft=path;
    PString pRight=path;  
    pLeft.Left(i);
    pRight.Left(len-offset);
    pRight.Right(len-i-1-offset);
    //LOGERR(("PathR: %s PathL:%s", (const char *)pRight  ,(const char *)pLeft));

    //Create a resource from proper paths
    return LoadDatabase(pRight,pLeft);
  }else{
    //Create the resource as best we can 
    return LoadDatabase(".",path);
  }
}

/*-------------------------------------------------------------------
PROCEDURE: LoadDatabase
PURPOSE  : To load a database from a given name and path.
PRE : TRUE
POST: Returns a ResourceDatabase that points to the name and path. 
-------------------------------------------------------------------*/
ResourceDatabase * ResourceDatabaseManager::LoadDatabase(PString name,PString path){
int len;
int numDB;
int i;
bool found;

  //Remove optional / or \ at the end of the path
  len=path.Length();

  if(path[len-1]=='/'||path[len-1]=='\\'){
    path.Left(len-1);
  }
 
  //Since windows is not case sensitivate
  #ifdef _WIN32
     path.ToLower();
     name.ToLower(); 
     //LOGERR(("Windows Path Names"));
  #endif

  //Search for an existing database
  numDB=names.Count();
  found=false;
  for (i=0;i<numDB;i++){
      if(names[i]==name && paths[i]==path){
          found=true;
          break;
      }
  }

  //If we found an existing DB
  if(found){
    //Increment reference count
    references[i]=references[i]+1;  

    //Return the pointer
    return databases[i];
  }else{

    //Create a new database
    ResourceDatabase * newDB = NewMem(ResourceDatabase(name,path));

    if(newDB==NULL){
        LOGERR(("ResourcesDatabases: Database not created Name:%s Path:",name,path));
    }

    //Add it to the list
    databases.Add(newDB);
    names.Add(name);
    paths.Add(path);
    references.Add(1);

    return newDB;
  }
}

/*-------------------------------------------------------------------
PROCEDURE: AddReference
PURPOSE  : To add a reference to the given database.
PRE : TRUE
POST: If the database is not found returns false, else returns true 
      and a reference is added for the database. 
-------------------------------------------------------------------*/
bool ResourceDatabaseManager::AddReference(const ResourceDatabase *db){
int i,numDB;

  
  //Search for the DB
  numDB=databases.Count();
  for (i=0;i<numDB;i++){
      if(databases[i]==db){
          references[i]=references[i]+1;
          return true;
      }
  }

 return false;
}

/*-------------------------------------------------------------------
PROCEDURE: RemoveReference
PURPOSE  : To remove a reference to the given database.
PRE : TRUE
POST: If the database is not found returns false, else returns true 
      and the database reference is decremented. If the database 
      reference =0 the database is deleted.
-------------------------------------------------------------------*/
bool ResourceDatabaseManager::RemoveReference(const ResourceDatabase *db){
int i,numDB;


  //Search for the DB
  numDB=databases.Count();
  for (i=0;i<numDB;i++){
      if(databases[i]==db){
          references[i]=references[i]-1;

          //If the refernce count is now 0 delete it (I would like 
          //  to but for some reason doing this can cause resource problems)
          
          if(references[i]==0){
             ResourceDatabase *tmp=databases[i]; 
             databases.Remove(i);
             names.Remove(i);
             paths.Remove(i);
             references.Remove((udword)i);
             DeleteMem(tmp);

          }
          
          return true;
      }
  }

 return false;

}


/*-------------------------------------------------------------------
PROCEDURE: DestroyAllDatabases
PURPOSE  : To remove all databases currently managed. Only call this 
           at the end of your program. (THIS IS A TEST METHOD)
PRE : TRUE
POST: All databases have been destroyed.
-------------------------------------------------------------------*/
void ResourceDatabaseManager::DestroyAllDatabases(){

   //Loop and destroy each one
   while(databases.Count()>0){
//     DeleteMem(databases[0]);
     databases.Remove(databases.First());
   }
   
   //Destroy all other arrays
   names.RemoveAll();
   paths.RemoveAll();
   references.RemoveAll(); 

 return;
}

 
