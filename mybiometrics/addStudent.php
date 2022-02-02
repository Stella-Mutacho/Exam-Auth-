<?php
 include('config.php');
  //save form data
  if (!empty($_POST['firstName'])){
	$firstName =  $_POST['firstName'];
	$lastName = $_POST['lastName'];
	$registrationNumber = $_POST['registrationNumber'];
   $fingerID = $_POST['fingerID'];
	
	$stmt = $db->prepare("INSERT INTO student(firstName, lastName, registrationNumber, fingerID) VALUES (?, ?, ?, ?)");
	$stmt->bind_param("ssss", $fname, $lname, $regno, $fID);

	// set parameters and execute
	$fname = $firstName;
	$lname = $lastName;
	$regno = $registrationNumber;
	$fID = $fingerID;

	if(!$stmt->execute()) echo "<span style='color:red'>Error while saving record</span>";
	else 
	$stmt->close();
	$db->close();
	header("location:index.php"); // redirects to users page
   echo "New Student added successfully";
   exit;
  }
?>

<!DOCTYPE html>
<html>
<head>
<title>Add Student</title>
<link href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-EVSTQN3/azprG1Anm3QDgpJLIm9Nao0Yz1ztcQTwFspd3yD65VohhpuuCOmLASjC" crossorigin="anonymous">
<div class="container">
<h3>Add a New Student</h3>
<style>
   body{background-color:#767c82;}
   .container{margin-top:3%;}
</style>
</head>
<body>
<form action="<?php echo $_SERVER['PHP_SELF'];?>" method="post">
      
<div class="row mb-4">
   <label for="inputFirstName" class="col-sm-1 col-form-label">First Name:</label>
   <div class="col-sm-3">
   <input type="firstName" id="inputFirstName" name="firstName" class="form-control">
   </div>
</div>
<div class="row mb-4">
   <label for="inputLastName" class="col-sm-1 col-form-label">Last Name:</label>
   <div class="col-sm-3">
   <input type="lastName" id="inputLastName" name="lastName" class="form-control" >
   </div>
</div>
<div class="row mb-4">
   <label for="inputRegistrationNumber" class="col-sm-1 col-form-label">Registration Number:</label>
   <div class="col-sm-3">
   <input type="registrationNumber" id="inputRegistrationNumber" name="registrationNumber" class="form-control" >
   </div>
</div>
<div class="row mb-4">
   <label for="fingerID" class="col-sm-1 col-form-label">Finger ID:</label>
   <div class="col-sm-3">
   <input type="fingerID" id="inputFingerID" name="fingerID" class="form-control" >
   </div>
</div>

	<button type="submit" class="btn btn-primary col-sm-1" name="login">Submit</button>

</form>
</div>
<script src="https://cdn.jsdelivr.net/npm/@popperjs/core@2.9.2/dist/umd/popper.min.js" integrity="sha384-IQsoLXl5PILFhosVNubq5LC7Qb9DXgDA9i+tQ8Zj3iwWAwPtgFTxbJ8NT4GN1R8p" crossorigin="anonymous"></script>
<script src="https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/js/bootstrap.min.js" integrity="sha384-cVKIPhGWiC2Al4u+LWgxfKTRIcfu0JTxR+EQDz/bgldoEyl4H0zUF0QKbrJ0EcQF" crossorigin="anonymous"></script>

</body>
</html>