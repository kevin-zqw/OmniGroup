// Copyright 2010-2014 Omni Development, Inc. All rights reserved.
//
// This software may only be used and reproduced according to the
// terms in the file OmniSourceLicense.html, which should be
// distributed with this project and can also be found at
// <http://www.omnigroup.com/developer/sourcecode/sourcelicense/>.
//
// $Id$

#import <UIKit/UIDocument.h>

#import <OmniFoundation/OFSaveType.h>
#import <OmniUIDocument/OUIDocumentPreview.h> // OUIDocumentPreviewArea

@class ODSFileItem, OUIDocumentViewController, OUIDocumentPreview;

@protocol OUIDocumentViewController;

@interface OUIDocument : UIDocument

+ (BOOL)shouldShowAutosaveIndicator;

- initWithExistingFileItem:(ODSFileItem *)fileItem error:(NSError **)outError;
// subclass this method if you need to set anything on the document after it's first been created from a template. (UUID's and the like)
- (id)initWithExistingFileItemFromTemplate:(ODSFileItem *)fileItem error:(NSError *__autoreleasing *)outError;
- initEmptyDocumentToBeSavedToURL:(NSURL *)url templateURL:(NSURL *)templateURL error:(NSError **)outError;
- initEmptyDocumentToBeSavedToURL:(NSURL *)url error:(NSError **)outError;
- initWithFileItem:(ODSFileItem *)fileItem url:(NSURL *)url error:(NSError **)outError;

// Can set this before opening a document to tell it that it is being opened for preview generation. Later we might want more control of how errors are captured for off-screen document work, but for now this just makes errors get logged instead of presented to the user. The document view controller may also opt to load less data or otherwise speed up its work by only doing what is necessary for preview generation.
@property(nonatomic) BOOL forPreviewGeneration;

@property(nonatomic,readonly) ODSFileItem *fileItem;

@property(nonatomic,readonly) UIViewController *viewControllerToPresent;
@property(nonatomic,readonly) UIViewController <OUIDocumentViewController> *documentViewController;
@property(nonatomic,readonly) BOOL editingDisabled;

@property(nonatomic,readonly) UIResponder *defaultFirstResponder; // Defaults to the documentViewController, or if that view controller implements -defaultFirstResponder, returns the result of that.

- (void)finishUndoGroup;
- (IBAction)undo:(id)sender;
- (IBAction)redo:(id)sender;

// Convenience properties that are only valid during a save operation (between -saveToURL:... and invocation of its completion handler
@property (readonly) UIDocumentSaveOperation currentSaveOperation; // see OFSaveTypeForUIDocumentSaveOperation
@property (readonly) NSURL *currentSaveURL;

// Called after an incoming rename, but before -enableEditing. Subclasses can refresh their references to child file wrappers. Called on a background queue via -performAsynchronousFileAccessUsingBlock:.
- (void)reacquireSubItemsAfterMovingFromURL:(NSURL *)oldURL completionHandler:(void (^)(void))completionHandler;

- (void)viewStateChanged; // Marks the document as dirty w/o logging an undo. If the app is backgrounded or the document closed it will be saved, but it won't be saved if the editor state change is the only change.
- (void)beganUncommittedDataChange; // Can be used when the user has started a change (like editing a value in a text field) to request that the value be autosaved eventually. This requires that the document subclass knows how to save the partial edits and that the act of doing so makes a real undoable change. Calling this for editor state changes can result in taps to Undo resulting in data loss in the case that you make UIDocument think it is back to its last saved state.

// Gives the document a chance to break retain cycles.
- (void)didClose;

// Subclass responsibility

/*
 self.fileItem and self.undoManager will be set appropriately when this is called. If fileItem is nil, this is a new document, but the UIDocument's fileURL will be set no matter what.
 */
- (UIViewController <OUIDocumentViewController> *)makeViewController;
- (void)updateViewControllerToPresent;

// Optional subclass methods
- (void)willFinishUndoGroup;
- (BOOL)shouldUndo;
- (BOOL)shouldRedo;
- (void)didUndo;
- (void)didRedo;
- (UIView *)viewToMakeFirstResponderWhenInspectorCloses;

- (NSString *)alertTitleForIncomingEdit;

- (id)tearDownViewController;
- (void)recreateViewControllerWithViewState:(id)viewState;
// When we get an incoming change from iCloud, OUIDocument discards the view controller it got from -makeViewController and makes a new one. These can be subclassed to help tear down the view controller and to transition view state from the old to the new, if appropriate.
- (NSDictionary *)willRebuildViewController;
- (void)didRebuildViewController:(NSDictionary *)state;

// Support for previews
+ (NSString *)placeholderPreviewImageNameForFileURL:(NSURL *)fileURL area:(OUIDocumentPreviewArea)area;
+ (void)writePreviewsForDocument:(OUIDocument *)document withCompletionHandler:(void (^)(void))completionHandler;

@end

extern OFSaveType OFSaveTypeForUIDocumentSaveOperation(UIDocumentSaveOperation saveOperation);

// A helper function to centralize the hack for -openWithCompletionHandler: leaving the document 'open-ish' when it fails.
// Radar 10694414: If UIDocument -openWithCompletionHandler: fails, it is still a presenter
extern void OUIDocumentHandleDocumentOpenFailure(OUIDocument *document, void (^completionHandler)(BOOL success));
