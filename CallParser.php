<?php
$command = './Parser ' . $_POST['wpmlfile'];
$myfile = fopen($_POST['filecreate'],"w");
exec($command, $theOutput, $status);
if ($status) {
  echo "Exec Error " . $status;
  echo "<br>Unfortunately the program was unable to run... Please try again from the Home page";
  echo <<<_END
<br><br>
<form method = "POST" action="index.php">
<input type="submit" value="Return Home"/>
</form>

_END;
} else {
  foreach($theOutput as $theLine) {
    $contents .= $theLine;
    $contents .= '
';
}
fwrite($myfile, $contents);
include $_POST['filecreate'];
}
?>