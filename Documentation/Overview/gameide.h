
/*! \page GameIDE Game IDE Module Overview

    <h2>Base Framework</h2>

    <ul>
        <li>clan::EditorMainWindow - Top-level window for the entire IDE</li>
		<li>clan::UIController - Binding layer between editors, documents, dockables and ribbon controls</li>
		<li>clan::UIControllerListener - Interface implemented by objects listening in on UIController events</li>
		<li>clan::DocumentEditorType - Interface implemented by document editor factories</li>
		<li>clan::DocumentEditor - Base class for document editor GUI components</li>
		<li>clan::DockableComponent - Base class for dockable GUI components</li>
    </ul>

    <h2>Rollouts</h2>

    <ul>
        <li>clan::RolloutList - Dockable GUI component listing properties</li>
        <li>clan::Rollout - GUI component for a property group</li>
    </ul>

    <h2>File Extensions</h2>

    <ul>
        <li>clan::FileItemType - Describes a file extension openable by the IDE open dialogs</li>
		<li>clan::FileItemTypeFactory - Factory that FileItemType objects needs to be registered to</li>
    </ul>

    <h2>Solution Model</h2>

    <ul>
        <li>clan::SolutionModel - Model class for an open solution in the IDE</li>
        <li>clan::Project - Model class for a project in a solution</li>
		<li>clan::ProjectItem - Base class for items in a project</li>
		<li>clan::ProjectRootItem - The root item in a project</li>
		<li>clan::ProjectFolderItem - A folder item linked to a physical path</li>
		<li>clan::ProjectFilterItem - A pseudo folder not reflected in the file system</li>
		<li>clan::ProjectFileItem - File item in the project</li>
		<li>clan::UserOptions - User specific state (opened items, etc)</li>
    </ul>

    <h2>Build System</h2>

    <ul>
        <li>clan::BuildSystem - Main class for the build system</li>
		<li>clan::BuildTool - Interface implemented by tools participating in the build</li>
        <li>clan::BuildOperation - BuildTool interfaces with this class to report build status</li>
    </ul>

*/
