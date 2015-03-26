<?php 
$Conn = odbc_connect("HandBook","","",SQL_CUR_USE_ODBC); //连接数据源 
$Doquery=odbc_exec($Conn,"select * from List");//执行查询 
var_dump($Doquery);
$a = odbc_fetch_array($Doquery);
var_dump($a);
$a = odbc_fetch_array($Doquery);
var_dump($a);

