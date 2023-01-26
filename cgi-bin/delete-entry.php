<?php
// Read the contents of the data.json file
$file = dirname(dirname(__FILE__)) . '/data/data.json';
$dataJson = file_get_contents($file);

// Decode the JSON data
$data = json_decode($dataJson, true);

// The data is now stored in the $data variable

// You can access the entries in the $data['entries'] array
$entries = $data['entries'];
//
$post_data_file = $argv[1];
$post_data = file_get_contents($post_data_file);
parse_str($post_data, $_POST);

// Get the form data
$id = $_POST['id'];

// Find the entry with the specified ID
$index = -1;
for ($i = 0; $i < count($entries); $i++) {
    if ($entries[$i]['id'] == $id) {
        $index = $i;
        break;
    }
}

// Delete the entry from the entries array
if ($index != -1) {
    array_splice($entries, $index, 1);
}

// Update the data object with the new entries array
$data['entries'] = $entries;

// Encode the data object as JSON
$dataJson = json_encode($data);

// Save the JSON data back to the data.json file
file_put_contents($file , $dataJson);

// Redirect back to the main page
// header('Location: /');
// exit;
?>

