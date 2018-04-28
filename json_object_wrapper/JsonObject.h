// Wrapper object around json_t pointer 
template<typename T>
class JsonObject
{
        public:
        // We will have parametrized container
        JsonObject(T* object): pointer_container(object)
        {
		release_pointer = NULL;
        }
	
	JsonObject()
	{
		pointer_container = NULL;	
		release_pointer = NULL;
	}
        // Object specific destructor which will call the user 
        // set call back function to release the allocated memory 
        ~JsonObject()
	{
		if(pointer_container)
			if(release_pointer)
				release_pointer(pointer_container);

	}

        // Destructor function for C pointer to release the memory
        // This will be the function pointer
        void (*release_pointer)(T *obj);

        // Overload the dereference operator to get the pointer 
        // to operate on native functions 
        T* operator*()
	{
		return pointer_container;
	}

	T& operator=(T *object)
	{
		if(object)
		{
			pointer_container = object;
			return *this;
		}

		throw "Bad allocation exception";
	}
        private:
        // Member variable to hold the original container
        T* pointer_container;
};      

