
require 'ruboto'

java_import "android.database.sqlite.SQLiteDatabase"

$db = SQLiteDatabase.new
$db.execSQL( "create table mytable (nbr integer, nam text)" )

