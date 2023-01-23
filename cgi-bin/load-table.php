<?php
    // Read the contents of the data.json file
    $file = dirname(dirname(__FILE__)) . '/data/data.json';
    $dataJson = file_get_contents($file);
    // Decode the JSON data
    $data = json_decode($dataJson, true);
    // The data is now stored in the $data variable
    // You can access the entries in the $data['entries'] array
    $entries = $data['entries'];

    // Iterate over the entries and output a row for each entry
    foreach ($entries as $entry) {
        echo "<tr>";
        echo "<td>" . $entry['id'] . "</td>";
        echo "<td>" . $entry['description'] . "</td>";
        echo "<td>" . $entry['price'] . "</td>";
        echo "<td>" . $entry['phone'] . "</td>";
        echo "</tr>";
    }
?>
