// Copyright 2010-2014 Omni Development, Inc. All rights reserved.
//
// This software may only be used and reproduced according to the
// terms in the file OmniSourceLicense.html, which should be
// distributed with this project and can also be found at
// <http://www.omnigroup.com/developer/sourcecode/sourcelicense/>.
//
// $Id$

#import <OmniUI/OUIWrappingViewController.h>
#import <OmniUIDocument/OUIDocumentPickerDelegate.h>

@class UINavigationController;
@class ODSStore, ODSScope, ODSFileItem, ODSFolderItem, OUIDocumentPickerViewController, OUIDocumentPickerScrollView;

@interface OUIDocumentPicker : OUIWrappingViewController

- (instancetype)initWithDocumentStore:(ODSStore *)documentStore;

@property (retain, nonatomic) ODSStore *documentStore;
@property (weak, nonatomic) id<OUIDocumentPickerDelegate> delegate;

- (void)showDocuments;
- (void)navigateToFolder:(ODSFolderItem *)folderItem animated:(BOOL)animated;
- (void)navigateToContainerForItem:(ODSItem *)item animated:(BOOL)animated;
- (void)navigateToScope:(ODSScope *)scope animated:(BOOL)animated;
- (ODSScope *)localDocumentsScope;

@property (nonatomic, readonly) OUIDocumentPickerViewController *selectedScopeViewController;

- (void)enableAppMenuBarButtonItem:(BOOL)enable;
@end
