/*
	container.hh
	------------
*/

#ifndef VXO_CONTAINER_HH
#define VXO_CONTAINER_HH

// vxo
#include "vxo/box.hh"


namespace vxo
{
	
	/*
		g++ 4.0 has a built-in operator[](<ptr>, int), which makes our own
		operator[](size_t) on Container and its derived classes ambiguous.
		So we define our own operator[](int) to disambiguate.
	*/
	
	class Container : public Box
	{
		public:
			typedef Box Item;
			
			static bool test( const Box& box );
			
			Container( box_type type, size_t n_items = 0 );
			
			size_t size() const  { return u.str.length; }
			
			Item* begin()  { return (Item*) u.str.pointer; }
			Item* end()    { return begin() + size(); }
			
			Item const* begin() const  { return (Item const*) u.str.pointer; }
			Item const* end()   const  { return begin() + size(); }
			
			Item const* cbegin() const  { return begin(); }
			Item const* cend()   const  { return end();   }
			
			Item&       operator[]( size_t i )        { return begin()[ i ]; }
			Item const& operator[]( size_t i ) const  { return begin()[ i ]; }
			
			Item&       operator[]( int i )        { return begin()[ i ]; }
			Item const& operator[]( int i ) const  { return begin()[ i ]; }
			
			Item&       front()        { return begin()[ 0 ]; }
			Item const& front() const  { return begin()[ 0 ]; }
			
			Item&       back()        { return end()[ -1 ]; }
			Item const& back() const  { return end()[ -1 ]; }
			
			void push_back( const Item& item )  { append( item ); }
			
			Item* append( const Item& item );
			Item* append_undef()  { return append( Box() ); }
	};
	
}

#endif
