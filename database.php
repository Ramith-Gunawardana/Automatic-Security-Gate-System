<!DOCTYPE html>
<html>
<head>
    <meta charset='utf-8'>
    <meta http-equiv='X-UA-Compatible' content='IE=edge'>
    <meta name='viewport' content='width=device-width, initial-scale=1'>
    <title>Automatic Security Gate - Log</title>

    <!-- Page Icon -->
    <link rel="icon" type="image/x-icon" href="/img/favicon.ico">
    
    <!-- Google fonts -->
    <link rel="preconnect" href="https://fonts.googleapis.com"><link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Roboto&display=swap" rel="stylesheet">

    <!-- Google icons -->
    <link rel="stylesheet" href="https://fonts.googleapis.com/css2?family=Material+Symbols+Outlined:opsz,wght,FILL,GRAD@48,400,0,0" />

    <!-- Date range picker -->
    <script type="text/javascript" src="https://cdn.jsdelivr.net/jquery/latest/jquery.min.js"></script>
    <script type="text/javascript" src="https://cdn.jsdelivr.net/momentjs/latest/moment.min.js"></script>
    <script type="text/javascript" src="https://cdn.jsdelivr.net/npm/daterangepicker/daterangepicker.min.js"></script>

    <!-- External CSS files -->
    <link rel="stylesheet" type="text/css" href="/daterange.css" />
    <link rel="stylesheet" href="/styles.css">

</head>

<body>
    <center><h2>Automatic Security Gate - Log</h2>
    
    <!-- Search Data in the table -->
    <div class="search">
    
    <p id="txtSeeRecentData">Show live preview of security camera</p>
       
       <p id="txtRefreshIcon" onClick="parent.open('https://gphw10-security.duckdns.org/mjpeg/1')">
           <span class="material-symbols-outlined">
               videocam
           </span>
           <span id="txtRefresh">Live Stream</span>
       </p>
       
       <hr id="hr">
    <p id="txtSeeRecentData">See recent data</p>

        <p id="txtRefreshIcon" onClick="window.location.reload();">
            <span class="material-symbols-outlined">
                refresh
            </span>
            <span id="txtRefresh">Refresh Page</span>
        </p>
        <hr id="hr">
        
        <p id="txtSearch">Search data</p>
        <div id="daterange">
            <span class="material-symbols-outlined">
                calendar_month
            </span>
            <span id="datetext"></span>
            <span class="material-symbols-outlined">
                expand_more
            </span>
        </div>
    </div> 
    <!-- <input type="text" id="serach_status" onkeyup="searchStatus()" placeholder="Search for staus..." title="Type to search"> -->
    
    <!-- End of search -->
     
    <!-- Table -->
    <table id="data_table" border=0>
        <tr class="table_head">
            <th class="thdate">Date</th>
            <th class="thtime">Time</th>
            <th class="thstatus">Status</th>
        </tr>

    <!----------- PHP Code to access database server ------------->
    <?php
        // error_reporting (E_ALL ^ E_NOTICE);

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

    
    try{
        $sql = "SELECT * from Data ORDER BY access_date DESC, access_time DESC";
        $result = mysqli_query($conn, $sql);
        
        if(mysqli_num_rows($result)>0){
            while($row = mysqli_fetch_assoc($result)){
                echo "<tr><td>".$row['access_date'] . "</td>" . "<td>".$row['access_time'] . "</td>" . "<td>".$row['id'] . "</td></tr>";
            }
        }else{
            echo "No data in the database";
        }
        
    }catch(mysqli_sql_exception $e){
        die("Data display error." . $e->getMessage());
    }
    echo "</table>";
    $conn->close();

    ?>
    <!---------------- End of PHP Code ------------------>
    <!-- <p id="txtNoSearchResult">No records found.</p> -->
    </center>
    <div class="copyright">
            <p>Copyright © Ramith Gunawardana. 2022 All Rights Reserved.</p>
    </div>


    <!--------- JavaScirpt Code to search data ----------->
    <script>
        var start_date, start_time, end_date, end_time, current_date, current_time;
        // var noSearchResult = document.getElementById('txtNoSearchResult');
        var dot;

        $(function() {
            var start = moment();
            var end = moment();

            var  table, tr, tdDate, tdTime, tdStatus, i, txtDate, txtTime, txtStatus;

            // function cb(start, end) {
            //     $('#daterange #datetext').html(start.format('MMMM D, YYYY (hh:mm A)') + ' - ' + end.format('MMMM D, YYYY (hh:mm A)'));
            // }
            function test() {
                $('#daterange #datetext').html('Click to apply date/time range');
            }
            function changeDotColor(){
                table = document.getElementById("data_table");
                tr = table.getElementsByTagName("tr");
        
                //run loop and visible only search results, other data is hidden
                for(i=0; i< tr.length; i++){
                    tdStatus = tr[i].getElementsByTagName("td")[2];
                    dot = tr[i];
                    if(tdStatus){
                        txtStatus = tdStatus.textContent || tdStatus.innerText;
                        
                        if(txtStatus.includes('Trespass')){
                            dot.style.color = "rgb(254, 79, 79)"; 
                        }else if(txtStatus.includes('VIP')){
                            dot.style.color = "rgb(110, 208, 254)";
                        }else if(txtStatus.includes('False Card')){
                            dot.style.color = "rgb(255, 199, 77)";
                        }else if(txtStatus.includes('Denied!')){
                            dot.style.color = "rgb(254, 79, 79)";
                        }else{
                            dot.style.color = "rgb(77, 255, 145)";
                        }
                    }
                }
            }
            $('#daterange').daterangepicker({
                opens: 'right',
                startDate: start.startOf('day').startOf('hour'),
                // startDate: start,
                endDate: end.endOf('day'),
                // endDate: end,
                "minYear": 2022,
                "timePicker": true,
                autoUpdateInput: true,
                ranges: {
                    'Today': [moment().startOf('day').startOf('hour'), moment().endOf('day')],
                    'Yesterday': [moment().startOf('day').subtract(1, 'days'), moment().endOf('day').subtract(1, 'days')],
                    'Last 7 Days': [moment().startOf('day').subtract(6, 'days'), moment().endOf('day')],
                    'Last 30 Days': [moment().startOf('day').subtract(29, 'days'), moment().endOf('day')],
                    'This Month': [moment().startOf('month'), moment().endOf('month')],
                    'Last Month': [moment().subtract(1, 'month').startOf('month'), moment().subtract(1, 'month').endOf('month')]
                },
                "minDate": "28/12/2022",
                "maxDate" : end.endOf('day'),
                "locale": {
                    "format": "DD/MM/YYYY"
                }
            }, 
            // cb, 
            // test
            );

            $('#daterange').on('apply.daterangepicker', function(ev, picker) {
                start_date = picker.startDate.format('YYYY-MM-DD');
                start_time = picker.startDate.format('HH:mm');
                end_date = picker.endDate.format('YYYY-MM-DD');
                end_time = picker.endDate.format('HH:mm');

                console.log('From : ' + start_date + ', ' +  start_time + ' To : ' + end_date + ', ' + end_time);

                start_date = start_date.split("-");
                start_time = start_time.split(":");
                end_date = end_date.split("-");
                end_time = end_time.split(":");

                // var dateObj_start = new Date(start_date[0], start_date[1], start_date[2]);
                // var dateObj_end = new Date(end_date[0], end_date[1], end_date[2]);
                var timeObj_start = new Date(start_date[0], start_date[1], start_date[2], start_time[0], start_time[1]);
                var timeObj_end = new Date(end_date[0], end_date[1], end_date[2], end_time[0], end_time[1]);

                table = document.getElementById("data_table");
                tr = table.getElementsByTagName("tr");

                //run loop and visible only search results, other data is hidden
                for(i=0; i< tr.length; i++){
                    tdDate = tr[i].getElementsByTagName("td")[0];
                    tdTime = tr[i].getElementsByTagName("td")[1];

                    if(tdDate){
                        txtDate = tdDate.textContent || tdDate.innerText;
                        txtTime = tdTime.textContent || tdTime.innerText;
                        
                        current_date = txtDate;
                        current_time = txtTime;
                        current_date = current_date.split("-");
                        current_time = current_time.split(":")

                        var dateObj_tableCurrent = new Date(current_date[0], current_date[1], current_date[2]);
                        var timeObj_tableCurrent = new Date(current_date[0], current_date[1], current_date[2], current_time[0], current_time[1]);
                        
                        // console.log(timeObj_start);
                        // console.log(timeObj_tableCurrent);
                        // console.log(timeObj_end);
                        if(timeObj_start <= timeObj_tableCurrent && timeObj_tableCurrent <= timeObj_end){
                            tr[i].style.display = ""; //row is visible
                            // noSearchResult.style.display = "none"; 
                        }else{
                            tr[i].style.display = "none"; //row is hidden
                            // noSearchResult.style.display = "block";
                        }
                    }
                }
                $('#daterange #datetext').html(picker.startDate.format('MMMM D, YYYY (hh:mm A)') + ' - ' + picker.endDate.format('MMMM D, YYYY (hh:mm A)'));
            });

            changeDotColor();
            // cb(start, end);
            test();
                
        });
    </script>
    <!--------- End of JavaScirpt Code --------->
    
</body>
</html>