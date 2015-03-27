#include "sqlite3.h"
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include "window.h"


#include <stdarg.h> //不定参数的应用要用到这个来接收参数

#ifndef DEBUG
    /* void DDebug(const char * str){  */
        // std::cout << str <<"\n";
    // }
    // void DDebug(std::map<std::string,std::string> Map)
    // {
        // std::cout << "debug --------------------> \n";
        // for(auto iter1 = Map.begin(); iter1 != Map.end(); ++iter1) 
        // {
            // std::cout << iter1->first << "=" << iter1->second << "\n";
        // }  
    /* }  */
#else
   /*  void Debug(const char * str){ */
    
    // }; 
    // void Debug(std::map<std::string,std::string> Map){
    
   /*  } */; 

#endif    

#ifndef SQL_CLASS
#define SQL_CLASS 1
//cl sql.cpp /I ../include ../lib/SQLITE3.lib /Fe:"../sql.exe" /EHsc
class Record
{
    public :
        std::map<std::string,std::string> field;
        void add(const char* fileName,const char * value)
        {
			if (value == '\0'){ value = "";}
            field[fileName] = value;
        }
        void clear()
        {
            field.clear();
        }
        std::string get(std::string fieldName)
        {
            return field[fieldName];
        }

};
class Result
{
    public :
        std::vector<Record> fields;
        Record Field;
        int i=0;
        bool setField(char* filedName, char* value)
        {
            // Field[std::string(filedName)] = std::string(value);
            Field.add(filedName,value);

           return true;
        }
        bool addRow()
        {
            fields.push_back(Field);
            Field.clear();
            return true;
        }
        Record* getone() 
        {
            if (i < fields.size()){
                return &fields[i++];
            }else{
                return 0;
            }
        } 
        void reset()
        {
            i=0;
            Field.clear();
            fields.clear();
        }
};
class sql
{
    private:
        char * errmsg;
		bool is_connect = false;
        Result  mResult ;
        sql(){};
        sql & operator = (const sql &);
		sqlite3_stmt * stmt = 0;
    public :
        sqlite3 *db;
        static sql & getInstance()
        {
            static sql instance;
            return instance;
        }
        /* sql(std::string filePath) */
        // {
            // //MessageBox(NULL,filePath.c_str(),"1",0);
            // connect(filePath);
        /* } */
        // sql(){}
        bool connect(std::string dbFile)
        {
			//if (is_connect)return true;
			int ret;
			char g_cmd_path[255];
			GetModuleFileNameA(NULL, g_cmd_path,255);
			strrchr(g_cmd_path, ('\\'))[1]=0;//删除文件名，只获得路径
			dbFile = g_cmd_path + std::string("/db/") + dbFile + ".db";
            ret = sqlite3_open(dbFile.c_str(),&db);
            if( ret ){   
                fprintf(stderr,"Can't open database: %s/n", sqlite3_errmsg(db));
                sqlite3_close(db);
                MessageBoxA(NULL,"connect db error","error",MB_OK);
                return false;   
            }
			
			//if (!is_connect)createTable();
			//is_connect = true;
            //DDebug("connect success;");
            return true;
        }

        Result * RESCULT() 
        {
            return &mResult;
        }

		sql * createCommand(std::string sql)
		{
			
			if (!sqlite3_prepare(db, sql.c_str(), -1, &stmt, 0)){
				MessageBoxA(NULL, "sqlite3_prepare", "error", MB_OK);
				return 0;
			}
			return this;
		}
		sql * bind(int pos_zero_indexed, std::string const& value)
		{
			if (sqlite3_bind_text(
				stmt,
				pos_zero_indexed + 1,  // Index of wildcard
				value.c_str(),
				value.length(),      // length of text
				SQLITE_TRANSIENT     // SQLITE_TRANSIENT: SQLite makes its own copy
				)
				!= SQLITE_OK) {
				MessageBoxA(NULL, "bind text error", "error", MB_OK);
				return 0;
			}
			return this;
		}
		sql * bind(int pos_zero_indexed, double value)
		{
			if (sqlite3_bind_double(
				stmt,
				pos_zero_indexed + 1,  // Index of wildcard
				value
				)
				!= SQLITE_OK) {
				MessageBoxA(NULL, "bind double error", "error", MB_OK);
				return 0;
			}
			return this;
		}

		bool query(std::string sql/* ,sqlite3_callback Xcallback  */, ...)
        {
            mResult.reset();
// 			va_list arg_ptr;
// 			va_start(arg_ptr, sql);
// 			sql = sqlite3_mprintf(sql.c_str(), arg_ptr);
// 			va_end(arg_ptr);

            int ret;
            ret = sqlite3_exec(db,sql.c_str(),&callback,static_cast<void*>(&mResult),&errmsg);
            if (! ret == SQLITE_OK){
				std::string errorstr;
				errorstr = "query fail! [" + std::string(sqlite3_errmsg(db)) + "][" + sql + "]";
                //sprintf(d,"query fail! [%s] [%s]",sql.c_str(),sqlite3_errmsg(db));
				MessageBoxA(NULL, errorstr.c_str(), "error", MB_OK);
                //Debug(d);
                return false;
            }
            return true;
        }
        int static callback(void*p_data ,int nCount,char** pValue,char** pName)
        {
            Result * pResult = reinterpret_cast<Result*>(p_data);
            for(int i=0; i< nCount; ++i)
            {
                pResult->setField(pName[i],pValue[i]);
                // std::cout << pName[i] << "=" << pValue[i] << "\n";
            }
            pResult->addRow();
            return 0;
        }

        bool findone(std::string sql);
        
        inline bool createTable()
        {
            std::string sql;
            sql = "create table if not exists main(id INTEGER PRIMARY KEY AUTOINCREMENT, root_id INTEGER ,title TEXT , content TEXT ,mean TEXT , create_time INTEGER  )";
            query(sql);
            sql = "create table if not exists root(id INTEGER PRIMARY KEY AUTOINCREMENT , title TEXT)";
			query(sql);
            return query(sql);

        }
        
};
//sql * PSQL = new sql();
#endif
/* int main() */
// {
    // sql db("D:/t");
    // db.createTable();
    // db.query("insert into main values(NULL,'22222222222','xxxxx',11111);");
    // db.query("select * from main;");

    // Record * precode;
    // while ( (precode = db.RESCULT()->getone() )){
     // debug(precode->get("id").c_str());
    // }
    // db.query("select * from main;");

    // while ( (precode = db.RESCULT()->getone() )){
     // debug(precode->get("id").c_str());
    // }
/* } */
