<?php
// Read the contents of the data.json file
$file = dirname(dirname(__FILE__)) . '/data/data.json';
$dataJson = file_get_contents($file);

// Decode the JSON data
$data = json_decode($dataJson, true);

// The data is now stored in the $data variable

// The headers of the post request need to be read from the tmp file
$post_data_file = $argv[1];
$post_data = file_get_contents($post_data_file);
parse_str($post_data, $_POST);

// You can access the entries in the $data['entries'] array
$entries = $data['entries'];

// Get the form data
$description = $_POST['description'];
$price = $_POST['price'];
$phone = $_POST['phone'];

// Generate a new ID for the entry
$usedIds = array_column($entries, 'id');
sort($usedIds);
for ($i = 1; $i <= count($entries) + 1; $i++) {
    if (!in_array($i, $usedIds)) {
        $id = $i;
        break;
    }
}

// Echo the id, so the jpeg file can be named
echo $id;

// Add the new entry to the entries array
$entries[] = array(
  'id' => (string)$id,
  'description' => $description,
  'price' => $price,
  'phone' => $phone,
);

// Update the data object with the new entries array
$data['entries'] = $entries;

// Encode the data object as JSON
$dataJson = json_encode($data);

// Save the JSON data back to the data.json file
file_put_contents($file , $dataJson);

// Redirect back to the main page
//header('Location: /');
//exit;
?>
