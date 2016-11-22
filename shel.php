<?php
    include('Net/SSH2.php');

    $server = "myserver";
    $username = "myadmin";
    $password = "mypass";
    $command = "ps";

    $ssh = new Net_SSH2($server);
    if (!$ssh->login($username, $password)) {
        exit('Login Failed');
    }

    echo $ssh->exec($command);
?>
