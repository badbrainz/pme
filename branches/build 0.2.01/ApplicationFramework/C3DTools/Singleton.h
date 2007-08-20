#ifndef SINGLETON
#define SINGLETON

template <class T> class Singleton
{
	public:
		 Singleton(const Singleton&){}
		 Singleton(){}
		~Singleton(){}
	
	public:
		Singleton& operator= (const Singleton&)
		{
			return *this;
		}

	public:
		static T& Instance()
		{
			static T instance;
			return instance;
	}
};

#endif