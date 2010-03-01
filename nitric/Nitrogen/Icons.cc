// Nitrogen/Icons.cc
// -----------------
//
// Maintained by Joshua Juran

// Part of the Nitrogen project.
//
// Written 2004-2009 by Lisa Lippincott and Joshua Juran.
//
// This code was written entirely by the above contributors, who place it
// in the public domain.


#include "Nitrogen/Icons.hh"

// Nitrogen
#include "Nitrogen/Folders.hh"
#include "Nitrogen/OSStatus.hh"


namespace Nitrogen
{
	
	// does nothing, but guarantees construction of theRegistration
	NUCLEUS_DEFINE_ERRORS_DEPENDENCY( IconManager )
	
	
	static void RegisterIconManagerErrors();
	
	
#if NUCLEUS_RICH_ERRORCODES
#pragma force_active on
	
	class IconManagerErrorsRegistration
	{
		public:
			IconManagerErrorsRegistration()  { RegisterIconManagerErrors(); }
	};
	
	static IconManagerErrorsRegistration theRegistration;
	
#pragma force_active reset
#endif
	
	
	Nucleus::Owned< CIconHandle > GetCIcon( ResID iconID )
	{
		CIconHandle h = ::GetCIcon( iconID );
		
		if ( h == NULL )
		{
			MemError();
			ResError();
			
			ThrowOSStatus( resNotFound );
		}
		
		return Nucleus::Owned< CIconHandle >::Seize( h );
	}
	
	void PlotCIcon( const Rect& rect, CIconHandle icon )
	{
		::PlotCIcon( &rect, icon );
	}
	
	PlainIconHandle GetIcon( ResID iconID )
	{
		// Returns a resource handle
		return Handle_Cast< PlainIcon >( Handle( ::GetIcon( iconID ) ) );
	}
	
	void PlotIcon( const Rect& rect, PlainIconHandle icon )
	{
		::PlotIcon( &rect, Handle( icon ) );
	}
	
	void PlotIconID( const Rect&        rect,
	                 IconAlignmentType  align,
	                 IconTransformType  transform,
	                 ResID              resID )
	{
		ThrowOSStatus( ::PlotIconID( &rect, align, transform, resID ) );
	}
	
	Nucleus::Owned< IconSuiteRef > NewIconSuite()
	{
		::IconSuiteRef result;
		ThrowOSStatus( ::NewIconSuite( &result ) );
		return Nucleus::Owned< IconSuiteRef >::Seize( IconSuiteRef( result ) );
	}
	
	void DisposeIconSuite( Nucleus::Owned< IconSuiteRef > iconSuite )
	{
		ThrowOSStatus( ::DisposeIconSuite( iconSuite.release(), true ) );
	}
	
	void DisposeIconSuite( Nucleus::Owned< IconSuiteRef, DisposeIconSuiteButNotData > iconSuite )
	{
		ThrowOSStatus( ::DisposeIconSuite( iconSuite.release(), false ) );
	}
	
	void PlotIconSuite( const Rect&        rect,
	                    IconAlignmentType  align,
	                    IconTransformType  transform,
	                    IconSuiteRef       iconSuite )
	{
		ThrowOSStatus( ::PlotIconSuite( &rect, align, transform, iconSuite ) );
	}
	
	static void PlotIconHandle_Internal( const Rect&        area,
	                                     IconAlignmentType  align,
	                                     IconTransformType  transform,
	                                     Handle             icon )
	{
		ThrowOSStatus( ::PlotIconHandle( &area, align, transform, icon ) );
	}
	
	void PlotIconHandle( const Rect&        area,
	                     IconAlignmentType  align,
	                     IconTransformType  transform,
	                     PlainIconHandle    icon )
	{
		PlotIconHandle_Internal( area, align, transform, icon );
	}
	
	void PlotIconHandle( const Rect&        area,
	                     IconAlignmentType  align,
	                     IconTransformType  transform,
	                     MaskedIconHandle   icon )
	{
		PlotIconHandle_Internal( area, align, transform, icon );
	}
	
	// The plot sickens...
	
	void PlotSICNHandle( const Rect&        area,
	                     IconAlignmentType  align,
	                     IconTransformType  transform,
	                     SmallIconHandle    theSICN )
	{
		ThrowOSStatus( ::PlotSICNHandle( &area, align, transform, Handle( theSICN ) ) );
	}
	
	void PlotCIconHandle( const Rect&        rect,
	                      IconAlignmentType  align,
	                      IconTransformType  transform,
	                      CIconHandle        theCIcon )
	{
		ThrowOSStatus( ::PlotCIconHandle( &rect, align, transform, theCIcon ) );
	}
	
   Nucleus::Owned<IconRef> GetIconRef( FSVolumeRefNum vRefNum, OSType creator, OSType iconType )
     {
      IconRef result;
      ThrowOSStatus( ::GetIconRef( vRefNum, creator, iconType, &result ) );
      return Nucleus::Owned<IconRef>::Seize( result );
     }

   GetIconRefFromFile_Result GetIconRefFromFile( const FSSpec& theFile )
     {
      IconRef icon;
      SInt16 label;
      ThrowOSStatus( ::GetIconRefFromFile( &theFile, &icon, &label ) );
      return GetIconRefFromFile_Result( Nucleus::Owned<IconRef>::Seize( icon ),
                                        IconLabel( label ) );
     }

   Nucleus::Owned<IconRef> GetIconRef( OSType creator, OSType iconType )
     {
      return GetIconRef( kOnSystemDisk, creator, iconType );
     }

   Nucleus::Owned<IconRef> GetIconRef( OSType iconType )
     {
      return GetIconRef( kOnSystemDisk, kSystemIconsCreator, iconType );
     }

	Nucleus::Owned<IconRef> GetIconRefFromFolder( FSVolumeRefNum vRefNum,
	                                     FSDirID parentFolderID,
	                                     FSDirID folderID,
	                                     FSIOFileAttributes attributes,
	                                     FSUserPrivileges accessPrivileges )
     {
      IconRef result;
      ThrowOSStatus( ::GetIconRefFromFolder( vRefNum,
                                             parentFolderID,
                                             folderID,
                                             attributes,
                                             accessPrivileges,
                                             &result ) );
      return Nucleus::Owned<IconRef>::Seize( result );
     }

   GetIconRefFromFileInfo_Result GetIconRefFromFileInfo( const FSRef&           inRef,
                                                         UniCharCount           inFileNameLength,
                                                         const UniChar         *inFileName,
                                                         FSCatalogInfoBitmap    inWhichInfo,
                                                         const FSCatalogInfo&   inCatalogInfo,
                                                         IconServicesUsageFlags inUsageFlags )
     {
      IconRef icon;
      SInt16 label;
      ThrowOSStatus( ::GetIconRefFromFileInfo( &inRef,
                                               inFileNameLength,
                                               inFileName,
                                               inWhichInfo,
                                               &inCatalogInfo,
                                               inUsageFlags,
                                               &icon,
                                               &label ) );
      return GetIconRefFromFile_Result( Nucleus::Owned<IconRef>::Seize( icon ),
                                        IconLabel( label ) );
     }

   GetIconRefFromFileInfo_Result GetIconRefFromFileInfo( const FSRef&           inRef,
                                                         UniCharCount           inFileNameLength,
                                                         const UniChar         *inFileName,
                                                         IconServicesUsageFlags inUsageFlags )
     {
      IconRef icon;
      SInt16 label;
      ThrowOSStatus( ::GetIconRefFromFileInfo( &inRef,
                                               inFileNameLength,
                                               inFileName,
                                               0,
                                               0,
                                               inUsageFlags,
                                               &icon,
                                               &label ) );
      return GetIconRefFromFile_Result( Nucleus::Owned<IconRef>::Seize( icon ),
                                        IconLabel( label ) );
     }

   Nucleus::Owned<IconRef> RegisterIconRefFromFSRef( OSType creator, OSType iconType, const FSRef& iconFile )
     {
      IconRef result;
      ThrowOSStatus( ::RegisterIconRefFromFSRef( creator, iconType, &iconFile, &result ) );
      return Nucleus::Owned<IconRef>::Seize( result );
     }

   Nucleus::Owned<IconRef> RegisterIconRefFromIconFile( OSType creator,
                                                        OSType iconType,
                                                        const FSSpec& iconFile )
	  {
      IconRef result;
      ThrowOSStatus( ::RegisterIconRefFromIconFile( creator, iconType, &iconFile, &result ) );
      return Nucleus::Owned<IconRef>::Seize( result );
     }
   
   void RegisterIconManagerErrors()
     {
      RegisterOSStatus< paramErr               >();
      RegisterOSStatus< memFullErr             >();
      RegisterOSStatus< memWZErr               >();
      RegisterOSStatus< resNotFound            >();
      RegisterOSStatus< noMaskFoundErr         >();
      RegisterOSStatus< invalidIconRefErr      >();
      RegisterOSStatus< noSuchIconErr          >();
      RegisterOSStatus< noIconDataAvailableErr >();
      RegisterOSStatus< afpIconTypeError       >();
     }
}

