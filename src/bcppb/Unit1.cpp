//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "dbtables.hpp"
#include "db.hpp"

/*#include <vcl\DBTables.h>*/
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

TDatabase* /*__fastcall TForm1::*/ RunTimeDbCreate(const AnsiString DatabaseName,
        const AnsiString SessionName, TComponent *owner)
{
  // If the session exists, make it active; if not, create a new session
  Sessions->OpenSession(SessionName);
  TSession *pSession = Sessions->FindSession(SessionName);
  TDatabase *TempDatabase = pSession->FindDatabase(DatabaseName);
  // if the database already exists, just return it
  if (TempDatabase)
    return TempDatabase;
 // Create a new database component
 TempDatabase =  new TDatabase(owner);

 try
 {
   TempDatabase->DatabaseName = DatabaseName;
   TempDatabase->SessionName = SessionName;
   TempDatabase->KeepConnection = true;
   return pSession->OpenDatabase(DatabaseName);
  }
  catch (...)
  {
    delete TempDatabase;
    throw;
  }
}

void __fastcall TForm1::Button1Click(TObject *Sender)
{


/*
	TSession *session = Sessions->FindSession ("Default");
	TDatabase *db = session->FindDatabase ("dbtest");
	if (!db)
	{
		db = new TDatabase (this);
		try
		{
			db->DatabaseName = "testdb";
			db->SessionName = "Default";
			db->KeepConnection = true;
			db = session->OpenDatabase ("testdb");
		}
		catch (...)
		{
			delete db;
			throw;
		}
	}
*/

        /*TDatabase *db = RunTimeDbCreate (AnsiString ("Lingala"), "", this);*/

        TSession *Session = Sessions->FindSession ("");

#define Alias "testdb4"

        try
        {
                Session->AddStandardAlias (Alias, "C:\\jacques\\bcppb\\testdb", "DBASE");
        }
        catch (EDatabaseError &DatabaseError)
        {
                Application->MessageBox (DatabaseError.Message.c_str(), "Error creating database", 0);
        };

        try
        {
                Session->SaveConfigFile();
        }
        catch (EDatabaseError &DatabaseError)
        {
                Application->MessageBox (DatabaseError.Message.c_str(),
                        "Error SaveConfigFile", 0);
        };

        /*
        TQuery *CreateTable = new TQuery (this);
        CreateTable->SQL->Close();
        CreateTable->SQL->DatabaseName = "testdb";
        CreateTable->SQL->Clear ();
        CreateTable->SQL->Add ("CREATE TABLE table1 (Reference INTEGER Description CHAR(30))");
        CreateTable->SQL->ExecSQL ();
        */

  	TTable *table1 = new TTable (this);
	table1->Active = false;
        table1->DatabaseName = Alias;
	table1->TableType = ttDefault;
	table1->TableName = "table1";
	table1->FieldDefs->Clear();
	/*
        table1->FieldDefs->Add("Ref", ftInteger, 0, True);
	table1->FieldDefs->Add("Des", ftString, 30, False);
	*/

        TFieldDef *field = table1->FieldDefs->AddFieldDef();
        field->DataType = ftInteger;
        field->Name = "Reference";
        field = table1->FieldDefs->AddFieldDef();
        field->DataType = ftString;
        field->Name = "Description";
        table1->IndexDefs->Clear();
	table1->CreateTable();

        TQuery *query1 = new TQuery (this);
        /*query1->SQL->Close();*/
        /* query1->SQL->DatabaseName = "testdb"; */
	query1->SQL->Clear ();
	query1->SQL->Add ("INSERT INTO \"table1\" (Reference, Description) VALUES (1234, 'This is the description')");
	query1->ExecSQL();

        TDatabase *db = Session->FindDatabase (Alias);
        /*db->Commit ();*/

        /*TDBGrid *grid = new TDBGrid (this);*/
        DataSource1->DataSet = table1;
        table1->Active = true;
}
//---------------------------------------------------------------------------
