<?php 
error_reporting(7);
/* $Conn = odbc_connect("HandBook","","",SQL_CUR_USE_ODBC); //连接数据源  */
// $Doquery=odbc_exec($Conn,"select * from List");//执行查询 
// var_dump($Doquery);
// $a = odbc_fetch_array($Doquery);
// var_dump($a);

//"F:\program\env\wamp\wamp\bin\php\php5.5 x86\php.exe" -c "F:\program\env\wamp\wamp\bin\php\php5.5 x86\php.ini"  D:\code\c\vclayout\bin\change.php

$odbc = new modbc(__DIR__."/php.mdb","","");

$sqlite = new sqlite(__DIR__."/db/db.db");

$all = $odbc->getall("select * from Root");
//var_dump($sqlite->getall("select * from root"));
foreach ($all as  $v)
{
    $sqlite->insert("root",array('title'=>$v['Root'],"id"=>$v['ID']));
}

$all = $odbc->getall("select * from List");
//var_dump($sqlite->getall("select * from root"));
foreach ($all as  $v)
{
    $sqlite->insert("main",array(
        'root_id'=>$v['RId'],
        'title' => $v['List'],
        'mean' => $v['Mean'],
        'content' =>$v['Remarks']
    ));
}

class sqlite
{
    var $db;
    function __construct($file,$username="",$password="")
    {
        $this->db=new PDO("sqlite:$file",$username,$password);
    }

    function getone($sql)
    {
        $rs = $this->query($sql);
        return $rs->fetch();
    }

    function getall($sql)
    {
        $rs = $this->query($sql);
        return $rs->fetchall();
    }

    function query($sql)
    {
        return $this->db->query($sql);
    }
  
    function insert($table,$field=array())
    {
        foreach ($field as $k=>$v)
        {          
            $key .= "$fh`$k`";
            $val .= "$fh'$v'";
            $str.="$fh`$k`='$v'";
            $fh=",";
        }

         $sql="INSERT INTO ".$table." ($key) VALUES ($val)";
        //echo "\n";
        $flag = $this->db->exec($sql); 
        //var_dump($flag);
        return $flag;
    }

    function update()
    {
    
    }

        

}
//var_dump($odbc->getall("select * from list"));
class modbc
{
    var $databasepath,$constr,$dbusername,$dbpassword,$link; 
    function __construct($databasepath,$dbusername,$dbpassword)
    {
        $this->databasepath=$databasepath; 
        $this->username=$dbusername; 
        $this->password=$dbpassword; 
        $this->connect();
    }
    
    function connect() 
    { 
        //$this->constr="DRIVER={Microsoft Access Driver (*.mdb)}; DBQ=" . realpath($this->databasepath);  
        $this->constr="HandBook";
        $this->link=odbc_connect($this->constr,$this->username,$this->password,SQL_CUR_USE_ODBC) or die("connect error");
        return $this->link; 
        //if($this->link) echo "恭喜你,数据库连接成功!"; 
        //else echo "数据库连接失败!"; 
    } 
    function query($sql)
    {
        return @odbc_exec($this->link,$sql);  
    }

    function insert($table,$field=array())
    {
        $temp=explode(',',$field); 
        $ins=''; 
        for ($i=0;$i<count($temp);$i++) 
        { 
            $ins.="'".$temp[$i]."',"; 
        } 
        $ins=substr($ins,0,-1); 
        $sql="INSERT INTO ".$table." (".$field.") VALUES (".$ins.")"; 
        $this->query($sql); 
    }

    function getone($sql)
    {
        $query = $this->query($sql);
        return odbc_fetch_array($query);
    }

    function getall($sql)
    {
        $row = array();
        $query = $this->query($sql);
        while ($a = odbc_fetch_array($query))
        {
            $row[] = $a;
        } 
        return $row;
    }

}
