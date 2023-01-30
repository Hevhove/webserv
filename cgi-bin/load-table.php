<?php
    // Read the contents of the data.json file
    $file = dirname(dirname(__FILE__)) . '/data/data.json';
    $dataJson = file_get_contents($file);
    // Decode the JSON data
    $data = json_decode($dataJson, true);
    // The data is now stored in the $data variable
    // You can access the entries in the $data['entries'] array
    $entries = $data['entries'];

    // Sort the entries by id
    usort($entries, function($a, $b) {
        return $a['id'] - $b['id'];
    });

    // Iterate over the entries and output a row for each entry
    foreach ($entries as $entry) {
        echo "<tr>";
        echo "<td>" . $entry['id'] . "</td>";
        echo "<td>" . $entry['description'] . "</td>";
        echo "<td>" . $entry['price'] . "</td>";
        echo "<td>" . $entry['phone'] . "</td>";
        echo "<td> <img src='images/{$entry['id']}.jpeg' width='100' height='100' alt='board picture'/>  </td>";
        echo "</tr>";
    }
?>
