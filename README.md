# Documentation & Basic Git Tutorial

## Git Tutorial

Github is a website that allows you to host your repositories online (remote) making it easier to
collaborate and share your source code, or even the executable.

First, clone the repository with the command:  
`git clone git@github.com:DeltaProcess/software_engineering_team_7_best_team.git`

It may ask you for an authentication key, you will want to authenticate your account over SSH using [this tutorial.](https://docs.github.com/en/authentication/keeping-your-account-and-data-secure/about-authentication-to-github#authenticating-with-the-command-line)

You can pull this repository with the command:  
`git pull`

Make changes to the code, such as correcting or adding to this documentation. To let git know that
you want to record when the file changes (staging), run the command:  
`git add <file_name>`

If you want to check what has been changed, or what is in the staging area, run the command:  
`git status`
^ This is highly recommended to use both before and after you add your files, so that you can see that you are changing the correct things.

Commit the changes (record them) with the command:  
`git commit -m "<changes_description>"`

Origin is the remote hosting site that the repository comes from, or pushes to.

Finally, push it back to the github repository (remote) so that we can see the changes
you made with the command:  
`git push <remote> <branch>`  
Which is generally:  
`git push origin main`

## Documentation
