<html>
<body>
<TITLE> Login </TITLE>
<H2>Login</H2>

<form method = "POST" action="CallParser.php">
	Username:<br>
	<input type="text" name="username"><br>
	<input type="hidden" name="wpmlfile" value="ChooseAStream.wpml">
	<input type="hidden" name="filecreate" value="ChooseAStream.php">
	<input type = "submit" value="Submit">
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

<?php

?>

</body>
</html>