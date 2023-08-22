<?php

    $host = "localhost";		        
    $dbname = "id20027910_db_security_gate";              
    $username = "id20027910_admin";		
    $password = "AutomaticSecurityGate-10";	     


// Establish connection to MySQL database
$conn = new mysqli($host, $username, $password, $dbname);


// Check if connection established successfully
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

else { echo "Connected to database. "; }

    // Get date and time variables
    date_default_timezone_set('Asia/Colombo');  // for other timezones, refer:- https://www.php.net/manual/en/timezones.asia.php
    $d = date("Y-m-d");
    $t = date("H:i:s");
    
    // If values send by NodeMCU are not empty then insert into MySQL database table
    $id = "";
    if(!empty($_POST['id'])){

        $id = $_POST['id'];
        // $sql = "INSERT INTO tricksumo_nodemcu (val, val2, Date, Time) VALUES ('".$val."','".$val2."', '".$d."', '".$t."')";
        try{
            $sql = "INSERT IGNORE INTO Data (access_date, access_time, id) VALUES ('".$d."', '".$t."', '".$id."')";
            $result = mysqli_query($conn, $sql);
            if ($conn->query($sql) === TRUE) {
                echo "Data inserted successfully";
            } else {
                echo "Error: " . $sql . "<br>" . $conn->error;
            }
            // echo "Data inserted successfully";
                
        }catch(mysqli_sql_exception $e){
                die("Data insert error." . $e->getMessage());
        }
    }

// Close MySQL connection
$conn->close();

?>
