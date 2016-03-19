
require 'ruboto'

java_import "android.database.sqlite.SQLiteDatabase"
java_import "android.database.sqlite.SQLiteOpenHelper"
java_import "android.database.sqlite.SQLiteDatabase.CursorFactory"

class MyBaseOpenHelper < SQLiteOpenHelper 

 def initialize( context, name, cf, version )
  super( context, name, cf, varsion )
 end

 def onCreate( db )
  db.execSQL( "create table mytable (nbr, nam)" )
 end

 def onUpgrade( db, old, new )
  db.execSQL( "drop table mytable" )
  onCreate( db )
 end
 
end

$myhelper = new MyBaseOpenHelper( nil, "mybase", nil, 1 )
$mybase = $myhelper.getWritableDatabase

$mybase.close

