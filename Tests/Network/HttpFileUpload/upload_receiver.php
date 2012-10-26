<?php
$uploaddir = '/Users/harry/uploads/';

foreach ($_FILES["Files"]["error"] as $key => $error) 
{
    if ($error == UPLOAD_ERR_OK) 
	{
        $tmp_name = $_FILES["Files"]["tmp_name"][$key];
        $name = $_FILES["Files"]["name"][$key];
        move_uploaded_file($tmp_name, "$uploaddir/$name");
    }
}

echo '$_FILES: ';
print_r($_FILES);

echo '$_POST: ';
print_r($_POST);
?>