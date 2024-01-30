# Configuration Python Functions 

## Functions

+ [string_to_list(list_string, delimiter)](py-api-create-utils-config-string-to-list)
+ [get_config(filenames, keep_case, verbose)](py-api-create-utils-config-get-config)

(py-api-create-utils-config-string-to-list)=
### string_to_list(list_string, delimiter=",")

Convert object as string into a list of objects.

If the object is already a list, return the list.

+ **list_string[in]** *(str)* List as string, e.g., "[a, b, c]".
+ **delimiter[in]** *(str)* Character(s) separating strings.
+ **returns** List of objects, e.g., ["a", "b", "c"]

(py-api-create-utils-config-get-config)=
### get_config(filenames, keep_case=True, verbose=False)

Get configuration from `.cfg` files.

+ **filenames[in]** *(list)* List of `.cfg` files to read.
+ **keep_case[in]** *(bool)* If True, maintain case in section headings, otherwise convert to lowercase.
+ **verbose[in]** *(bool)* If True, print out progress.
+ **returns** Dictionary with configuration.
