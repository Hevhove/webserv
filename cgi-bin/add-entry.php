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
$id = count($entries) + 1;

//Handle the file upload
$target_dir = "public/www/images/";
$target_file = $target_dir . $id . ".jpeg";
$file_type = pathinfo($target_file, PATHINFO_EXTENSION);

// Allow certain file formats
if($file_type != "jpeg" && $file_type != "png") {
    echo "Sorry, only JPEG and PNG files are allowed.";
    exit;
}

// Check if file already exists
if (file_exists($target_file)) {
    echo "Sorry, file already exists.";
    exit;
}

// Check file size
if ($_FILES["picture"]["size"] > 500000) {
    echo "Sorry, your file is too large.";
    exit;
}

// Check if $uploadOk is set to 0 by an error
if (move_uploaded_file($_FILES["picture"]["tmp_name"], $target_file)) {
    echo "The file ". basename( $_FILES["picture"]["name"]). " has been uploaded.";
} else {
    echo "Sorry, there was an error uploading your file.";
    exit;
}

// Add the new entry to the entries array
$entries[] = array(
  'id' => $id,
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
header('Location: /');
exit;
?>
