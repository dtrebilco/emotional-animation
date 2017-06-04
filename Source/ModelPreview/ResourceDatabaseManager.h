/*--------------------------------------------------------------------
  ResourceDatabaseManager.h

  Contains the definition a class used to manage the creation and 
  allocation of resource databases. (Came about as you cannot have a 
  resource database pointing to the same directory and then delete both)

  21/03/2001 -- Created the file

By Damian Trebilco
   Copyright © 2001. All Rights Reserved.
--------------------------------------------------------------------*/

#ifndef _INCLUDED_RESOURCE_DATABASE_MANAGER_H_
#define _INCLUDED_RESOURCE_DATABASE_MANAGER_H_

#include "stdafx.h"
#include <ResourceDatabase.h>
#include <FastDynamicArray.h>
#include <DynamicArray.h>
#include <PString.h>

using namespace Jet;

class ResourceDatabaseManager  
{

public:

    //Constructor/Destructor
	ResourceDatabaseManager();
	virtual ~ResourceDatabaseManager();

    //New methods
    bool AddReference(const ResourceDatabase *db);
    bool RemoveReference(const ResourceDatabase *db);

    //Load from an entire path
    ResourceDatabase * LoadDatabase(PString path);

    //Load from the path and name
    ResourceDatabase * LoadDatabase(PString name,PString path);

    void DestroyAllDatabases();

private:
    //A variable to count the number of times this class loads
    static unsigned int numLoadingRDB;

    typedef FastDynamicArray<ResourceDatabase *> DBArray;
    typedef FastDynamicArray<int> DBReferences;
    typedef DynamicArray<PString,16> DBNameArray;
    typedef DynamicArray<PString,16> DBPathArray;

    //The array to store the database pointers
    static DBArray databases;

    //The array to store the database references
    static DBReferences references;

    //The arrays to store where the DB's came from
    static DBNameArray names;
    static DBPathArray paths;

};

#endif 
