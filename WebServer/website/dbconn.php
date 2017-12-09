<?php
/**
 * Created by PhpStorm.
 * User: davidkopala
 * Date: 11/8/17
 * Time: 9:30 PM
 */

$conn = mysqli_connect("localhost", "admin", "password", "ECEN1400");

if(strcmp($_SERVER['REQUEST_METHOD'], "GET") == 0) {
    get();
} else if(strcmp($_SERVER['REQUEST_METHOD'], "POST") == 0) {
    post();
} else if(strcmp($_SERVER['REQUEST_METHOD'], "PUT") == 0) {

} else if(strcmp($_SERVER['REQUEST_METHOD'], "DELETE") == 0) {

}

//?table=table_name&type=A|B&timestamp=<xxxx||>yyyy
function get() {
    global $conn;

    if (!isset($_GET['table'])) {
        die("BAD");
    }

    $reserved = ["table", "dir", "order", "limit"];

    $where = "";
    foreach ($_GET as $key => $value) {
        if (!in_array($key, $reserved)) {
            $where .= "( ";
            $ands = explode('||', $value);
            foreach ($ands as $and_val) {
                $ors = explode('|', $and_val);
                foreach ($ors as $or_val) {
                    $cond = "=";
                    $val = "";
                    if ((strpos($or_val, '<') !== false) || (strpos($or_val, '>') !== false)) {
                        $cond = substr($or_val, 0, 1);
                        $val = substr($or_val, 1);
                    } else {
                        $val = $or_val;
                    }
                    if (!is_numeric($val)) {
                        $val = "'" . $val . "'";
                    }
                    $where .= $key . " " . $cond . " " . $val . " OR ";
                }
                $where = substr($where, 0, -4);
                $where .= " AND ";
            }
            $where = substr($where, 0, -5);
            $where .= " ) AND ";
        }
    }
    $where = substr($where, 0, -5);
    if (strlen($where) > 0) {
        $where = " WHERE " . $where;
    }
    $query = "SELECT * FROM " . $_GET['table'] . $where;
    if(isset($_GET['order'])) {
        $query .= " ORDER BY " . $_GET['order'];
        if (isset($_GET['dir'])) {
            if(strcmp($_GET['dir'], "ASC") == 0) {
                $query .= " ASC";
            } else if(strcmp($_GET['dir'], "DESC") == 0) {
                $query .= " DESC";
            }
        }
    }
    if (isset($_GET['limit'])) {
        if (is_numeric($_GET['limit'])) {
            $query .= " LIMIT " . $_GET['limit'];
        }
    }
    //echo $query;
    $results = mysqli_query($conn, $query);
    if (!$results) {
        echo "bad result<br>";
        echo $query . "<br>";
        var_dump($conn->connect_error);
    }
    echo stringify_results($results);
}

function stringify_results($results) {
    $JSON = "[";
    $fields = $results->fetch_fields();
    while ($row = mysqli_fetch_row($results)) {
        $JSON .= '{';
        for ($i = 0; $i < count($row); ++$i) {
            $JSON .= '"'.$fields[$i]->name.'":"'.$row[$i].'",';
        }
        $JSON = substr($JSON, 0, -1);
        $JSON .= '},';
    }
    $JSON = substr($JSON, 0, -1);
    $JSON .= ']';
    if(strcmp($JSON, "]") == 0) {
        return "[]";
    }
    return $JSON;
}

function post() {
    global $conn;

    if (!isset($_GET['table'])) {
        return;
    }

    $keys = "(  ";
    $values = "(  ";
    foreach ($_POST as $key => $value) {
        $keys .= '`' . $key . '`, ';
        if (is_numeric($value)) {
            $values .= $value . ", ";
        } else {
            $values .= "'" . $value . "', ";
        }
    }
    $keys = substr($keys, 0, -2) . ' )';
    $values = substr($values, 0, -2) . ' )';

    $query = "INSERT INTO " . $_GET['table'] . $keys . " VALUES " . $values;
    mysqli_query($conn, $query);

    $r_query = "SELECT * FROM " . $_GET['table'] . " WHERE `ID`=LAST_INSERT_ID()";
    $result = mysqli_query($conn, $r_query);

    echo stringify_results($result);
}