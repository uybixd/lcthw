#!/bin/bash
# Ensure the program aborts if an error occurs
set -e

# Test Each fucntion

# The first is Database_creat

source_file="$1"

executable="${source_file%.c}"

cc "$source_file" -o "$executable"
echo "Compiling $source_file sucess"

"./$executable" "test.db" "c"
echo "Databse_create success."

"./$executable" "test.db" "s" "0" "Jimmy" "test@test.com"
echo "Databse_set success."

"./$executable" "test.db" "l"
echo "Database_list success."

"./$executable" "test.db" "g" "0"
echo "Database_get success."

#"./$executable" "test.db" "f" "Jimmy"
#echo "Database_find success."

"./$executable" "test.db" "d" "0"
echo "Database_delete success."
# clean the test database
rm test.db
rm "$executable"