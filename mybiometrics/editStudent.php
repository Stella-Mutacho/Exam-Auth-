<?php

include('myconfig.php'); // Using database connection file here

$id = $_GET['id']; // get id through query string

$query = mysqli_query($db,"SELECT * FROM students WHERE studentID = ".$id); // select query

$data = mysqli_fetch_array($query); // fetch data

if(isset($_POST['update'])) // when click on Update button
{
    $firstName = $_POST['firstName'];
    $lastName = $_POST['lastName'];
    $registrationNumber = $_POST['registrationNumber'];
    $fingerID = $_POST['fingerID'];
	
    $edit = mysqli_query($db,"UPDATE students SET firstName='$firstName', lastName='$lastName', registrationNumber='$registrationNumber', 
    fingerID='$fingerID' where studentID ='$id'");
	
    if($edit)
    {
      $db->close(); // Close connection
        header("location:index.php"); // redirects to all users page
        exit;
    }
    else
    {
        echo mysqli_connect_error();
    }    
    
}
?>

<link href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-EVSTQN3/azprG1Anm3QDgpJLIm9Nao0Yz1ztcQTwFspd3yD65VohhpuuCOmLASjC" crossorigin="anonymous">
<div class="container">
<h3>Update Student Details</h3>
<style>
	body{background-color:#767c82;}
   .container{margin-top:3%;}
</style>
<form method="POST">
   
<div class="row mb-4">
   <label for="inputFirstName" class="col-sm-1 col-form-label">First Name:</label>
   <div class="col-sm-3">
   <input type="text" class="form-control"  name="firstName" value="<?php echo $data['firstName'] ?>" >
   </div>
</div>
<div class="row mb-4">
   <label for="inputLastName" class="col-sm-1 col-form-label">Last Name:</label>
   <div class="col-sm-3">
   <input type="text" class="form-control" name="lastName" value="<?php echo $data['lastName'] ?>" >
   </div>
</div>
<div class="row mb-4">
   <label for="inputRegistrationNumber" class="col-sm-1 col-form-label">Registration Number:</label>
   <div class="col-sm-3">
   <input type="text" class="form-control"  name="registrationNumber" value="<?php echo $data['registrationNumber'] ?>" >
   </div>
</div>
<div class="row mb-4">
   <label for="inputFingerID" class="col-sm-1 col-form-label">Finger ID:</label>
   <div class="col-sm-3">
   <input type="text" class="form-control" name="fingerID" value="<?php echo $data['fingerID'] ?>" >
   </div>
</div>

<button type="submit" class="btn btn-primary col-sm-1" name="update">Update</button>

</form>

</div>
<script src="https://cdn.jsdelivr.net/npm/@popperjs/core@2.9.2/dist/umd/popper.min.js" integrity="sha384-IQsoLXl5PILFhosVNubq5LC7Qb9DXgDA9i+tQ8Zj3iwWAwPtgFTxbJ8NT4GN1R8p" crossorigin="anonymous"></script>
<script src="https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/js/bootstrap.min.js" integrity="sha384-cVKIPhGWiC2Al4u+LWgxfKTRIcfu0JTxR+EQDz/bgldoEyl4H0zUF0QKbrJ0EcQF" crossorigin="anonymous"></script>
