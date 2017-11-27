<html>
<body>

<?php
	$flag = $_POST['flag'];
	if ($flag == 'st@rt' || $flag == 'nam3sort') {
		$savedFlag = $flag;
	} else {
		$savedFlag = $_POST['savedflag'];
	}
	if ($flag == 'switchSort') {
		if ($savedFlag == 'st@rt') {
			$savedFlag = 'nam3sort';
			$flag = 'nam3sort';
		} elseif ($savedFlag == 'nam3sort') {
			$savedFlag = 'st@rt';
			$flag = 'st@rt';
		} 
	}
	if ($flag == 'st@rt') {
		$offset = 0;
		$lastpostread = 0;
	} else {
		$lastpostread = $_POST['lastpostread'];
		$offset = $_POST['offnum'];
	}
	if ($_POST['offsetplus'] == 'true' && $offset < 1) {
		$offset = 1;
	}
	if ($_POST['offsetminus'] == 'true' && $offset > -1) {
		$offset = -1;
	}
	$uname = $_POST['username'];
	$sname = $_POST['stream'];
	$cmd = './view.py ' . $_POST['username'] . ' un1qu3Ph@s3 ' . $_POST['stream'] . ' -fl@g ' . $flag . ' ' . $offset . ' ' .    $lastpostread . ' ' . $savedFlag;
	$count = 0;
	exec($cmd, $output, $status);
	if ($status) {
	  echo "Exec Error " . $status;
	  echo "<br>Unfortunately the program was unable to run... Please try again from the Home page";
	} else {
		echo "<H2>Stream " . $_POST['stream'] . " </H2>";
		echo <<<_END

<hr>

<form style = 'float: left' method = "POST" action="view.php">
<input type="hidden" name="username" value="$uname">
<input type="hidden" name="stream" value="$sname">
<input type="hidden" name="savedflag" value="$savedFlag">
<input type="hidden" name="flag" value="switchSort">
<input type="hidden" name="offnum" value="$offset">
<input type="hidden" name="lastpostread" value="$lastpostread">
<input type = "submit" value="Change Sorting Method">
</form>
<form style = 'float: left' method = "POST" action="view.php">
<input type="hidden" name="username" value="$uname">
<input type="hidden" name="stream" value="$sname">
<input type="hidden" name="flag" value="-m@ll">
<input type="hidden" name="savedflag" value="$savedFlag">
<input type="hidden" name="offnum" value="$offset">
<input type="hidden" name="lastpostread" value="$lastpostread">
<input type = "submit" value="Mark All Read">
</form>
<form style = 'float: left' method = "POST" action="CallParser.php">
<input type="hidden" name="wpmlfile" value="ChooseAStream.wpml">
<input type="hidden" name="filecreate" value="ChooseAStream.php">
<input type="hidden" name="username" value="$uname">
<input type = "submit" value="Change Stream">
</form>
<form style = 'float: left' method = "POST" action="view.php">
<input type="hidden" name="username" value="$uname">
<input type="hidden" name="stream" value="$sname">
<input type="hidden" name="flag" value="st@rt">
<input type="hidden" name="savedflag" value="$savedFlag">
<input type="hidden" name="lastpostread" value="$lastpostread">
<input type = "submit" value="Check for new Posts">
</form>
<form style = 'float: left' method = "POST" action="CallParser.php">
<input type="hidden" name="wpmlfile" value="AddAuthor.wpml">
<input type="hidden" name="filecreate" value="AddAuthor.php">
<input type = "submit" value="Add Author">
</form>
<form style = 'float: left' method = "POST" action="CallParser.php">
<input type="hidden" name="wpmlfile" value="RemoveAuthor.wpml">
<input type="hidden" name="filecreate" value="RemoveAuthor.php">
<input type = "submit" value="Remove Author">
</form>
<form style = 'float: left' method = "POST" action="index.php">
<input type = "submit" value="Change Author">
</form>

_END;

if ($_POST['stream'] != 'all') {
	echo <<< _END
<form style = 'float: left' method = "POST" action="CallParser.php">
<input type="hidden" name="wpmlfile" value="Post.wpml">
<input type="hidden" name="filecreate" value="Post.php">
<input type="hidden" name="username" value="$uname">
<input type="hidden" name="stream" value="$sname">
<input type = "submit" value="Post to Current Stream">
</form> 
_END;
}

echo <<< _END
<p align="left"> <BR> <hr> </p>
_END;

	$arraysize = sizeof($output);
	$count = 1;
	foreach($output as $line){
		if ($count != $arraysize) {
			echo $line;
			echo "<br>";
		} else {
			$lastpostread = $line;
		}
		$count = $count + 1;
	}
echo <<<_END
<br><br><hr>
<form style = 'float: left' method = "POST" action="view.php">
<input type="hidden" name="username" value="$uname">
<input type="hidden" name="stream" value="$sname">
<input type="hidden" name="flag" value="-0ffS3t">
<input type="hidden" name="offsetminus" value="true">
<input type="hidden" name="savedflag" value="$savedFlag">
<input type="hidden" name="offnum" value="$offset">
<input type="hidden" name="lastpostread" value="$lastpostread">
<input type = "submit" value="Previous Post">
</form>
<form style = 'float: left' method = "POST" action="view.php">
<input type="hidden" name="username" value="$uname">
<input type="hidden" name="stream" value="$sname">
<input type="hidden" name="flag" value="-0ffS3t">
<input type="hidden" name="savedflag" value="$savedFlag">
<input type="hidden" name="offsetplus" value="true">
<input type="hidden" name="offnum" value="$offset">
<input type="hidden" name="lastpostread" value="$lastpostread">
<input type = "submit" value="Next Post">
</form> 
_END;
}

echo <<<_END
<p alighn = "left"> <br><br> </p>
<form method = "POST" action="index.php">
<input type = "submit" value="Home">
</form>
_END;

	if ($savedFlag == 'nam3sort') {
		echo "<br> Currently Sorting by: <b>Name</b><br><br>";
	} else {
		echo "<br> Currently Sorting by: <b>Date</b><br><br>";
	}
?>

</body>
</html>