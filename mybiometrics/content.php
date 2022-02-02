<html>
    <head>
        <?php require 'myconfig.php'; //connect to the database ?>
        <?php
        //query the database
        $sql = "select * from content";
        $result = mysqli_query($db,$sql);

        if (!$result) {echo "Error in your query:".mysql_error();exit;}

        $row = mysqli_fetch_row($result);
        $title = $row[1]; //title column of the table content
        $body = $row[2]; //body column of the table content
        ?>

        <title><?php echo $title; ?></title>
        
    </head>

    <body>
        <h1><?php echo $title; ?></h1>
        <?php
        echo $body;
        ?>
    </body>
</html>