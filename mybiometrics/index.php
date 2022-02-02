<!DOCTYPE html>
<html>
<head>
<?php require 'config.php'; //connect to the database?>
<title>Manage Students</title>
<link href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-EVSTQN3/azprG1Anm3QDgpJLIm9Nao0Yz1ztcQTwFspd3yD65VohhpuuCOmLASjC" crossorigin="anonymous">
<style>
	body{background-color:#767c82;}
   .container{margin-top:3%;}
</style>
</head>

<body>
<div class="container">
<h3>Students List</h3>

<div class="row mb-4">
<a href = "addStudent.php" style="color:#fff">Add student</a>
</div>

<table class="table table-dark table-striped table-hover table-bordered">
<th>Students List</th>
<tr> <th>First Name</th> <th>Last Name</th> <th>Registration Number</th> <th>Finger ID</th> </tr>

<?php
	$str = '';
	  $sql = "SELECT * FROM students";
      $result = mysqli_query($db,$sql);
	  $count = mysqli_num_rows($result);
	  if ($count==0) $str .= "<tr><td colspan='5'><span class='alert alert-danger'>No users found.</span></td></tr>";
	  else
      while ($row = mysqli_fetch_array($result,MYSQLI_ASSOC)){
		$str .= "<tr>
		<td>".$row['firstName']."</td>
		<td>".$row['lastName']."</td>
		<td>".$row['registrationNumber']."</td>
		<td>".$row['fingerID']."</td>
		<td>
		<a href='editStudent.php?id=".$row['studentID']."'>Edit</a>
		</td></tr>";		
	  }
	echo $str;

?>

</table>
</div>

<script src="https://cdn.jsdelivr.net/npm/@popperjs/core@2.9.2/dist/umd/popper.min.js" integrity="sha384-IQsoLXl5PILFhosVNubq5LC7Qb9DXgDA9i+tQ8Zj3iwWAwPtgFTxbJ8NT4GN1R8p" crossorigin="anonymous"></script>
<script src="https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/js/bootstrap.min.js" integrity="sha384-cVKIPhGWiC2Al4u+LWgxfKTRIcfu0JTxR+EQDz/bgldoEyl4H0zUF0QKbrJ0EcQF" crossorigin="anonymous"></script>

</body>
</html>