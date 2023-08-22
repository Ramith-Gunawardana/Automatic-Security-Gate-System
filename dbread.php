<?php
    error_reporting (E_ALL ^ E_NOTICE);
    
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

else { //echo "Connected to mysql database. <br>"; 
}

$id = "";
//Get POST values from NodeMcu
if($_POST['id']){
    $id = $_POST['id'];
}else{
    echo "Card not detected.";
}

// Select values from MySQL database table
/*
$sql = "SELECT name FROM Tags where id='$id'"; 

$result = $conn->query($sql);



if ($result->num_rows > 0) {
    // output data of each row
    while($row = $result->fetch_assoc()) {
        echo "<b>Id: </b>" . $row["id"]. " &nbsp <b>Name:</b>" . $row["name"]. "<p>";
    }
} else {
    echo "0 results";
}
*/

try{
    $sql = "SELECT name FROM Tags where id='$id' LIMIT 1";
    $result = mysqli_query($conn, $sql);
    
    
    if(mysqli_num_rows($result)>0){
        while($row = mysqli_fetch_assoc($result)){
            echo $row['name'] . "," . "$id";
            // var_dump($row);
        }
    }else{
        echo "null";
    }
    
}catch(mysqli_sql_exception $e){
    die("Data display error." . $e->getMessage());
}
$conn->close();

?>
