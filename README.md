### AVL tree library

This is a C89 library to provide an AVL tree implementation for when you don't
have one present in your run-time system already. It emphasizes the following:

* Standard C89 for portability
* Small code size (less than 350 lines of source code, 12KB of x86_64 code when compiled)
* Good performance (beats C++ STL std::map<> in many cases)

It has the following additional functionality:

* Easy to integrate into your own codebase, with an easily-customizable prefix
    on all function and type names
* Not dependent on the standard C allocator --- can be used with whatever
    allocator you like

It has been used in:

* Operating system kernels
* Linkers and dynamic loaders
* Plus various systems research projects

#### Sample use

    /* Map string names to malloc()'d blocks. */
    avl_tree_t tree;
    /* Cast strcmp to avoid warnings; avl_comparator_t takes two void *
        arguments, strcmp wants two const char *s.
        free() is used as the key destructor. */
    avl_initialize(&tree, (avl_comparator_t)strcmp, free);

    for(int i = 0; i < 100; i ++) {
        const char *name = get_next_name();
        /* avl_insert will return the old data if something already exists with
            the specified key, and call the key destructor on the new key. */
        avl_insert(&tree, strdup(name), malloc(sizeof(record_t)));
    }

    /* Can now search at will. strcmp will be used to compare keys. */
    record_t *data = avl_search(&tree, "Ebenezer Scrooge");
    /* Will return NULL if not found.
       Also returns NULL if that's what the key maps to. */
    data = avl_search(&tree, some_string());

    /* Let's get rid of some things. */
    /* avl_remove will call free() on the key (since we specified it as a key
        destructor) but won't touch the data, it'll just return what it found
        at that key, if anything, or NULL otherwise. Since free(NULL) is a
        no-op...*/
    free(avl_remove(&tree, "James Bond"));
    free(avl_remove(&tree, "Alfred E. Neuman"));

    /* Now it's time to get rid of the entire tree. The second argument is a
        function to be called on each node in order to clean up what the
        relevant pointers reference; we'll just use a built-in function here
        that frees both the keys and the data they point to. */
    avl_destroy(&tree, avl_free_data);
