# Lily58 bépo-dev keyboard

## Purpose
Personal keyboard layout for the Lily58, starting from  bépo (https://bepo.fr/wiki/Accueil) and tuned for development and for writing in French and English.

## Hardware
The built firmware is designed for the following hardware:

- splitkb Aurora Lily58 rev1 (https://splitkb.com/collections/keyboard-kits/products/aurora-lily58)
- Liatris controllers
- 2 encoders (one on each half at the position SW19)
- 2 OLED screens
- per-key RGB
- RGB underglow

## Host requirement
The firmware is built to work on devices with the keyboard layout set to Belgian French (AZERTY).

## Upgrading QMK
Follow the steps to upgrade QMK:

- choose a version to upgrade to (a branch to always receive the newest changes but it might break often, or a tag)
- Update the `qmk_ref: <tag>` to the version you selected in the previous step
- commit the changes
- go to your local `qmk-firmware` copy and run the following command:

        git checkout <tag>

- return to your `qmk_userspace` folder and compile
- run all the tests
- if everything went well, you can push your changes to GitHub and check if the workflow passes too

## Status
*Work in progress*

For a complete view on the progress refer to the Jira Project "DEV".

## Code quality

Check the keymap with QMK's linter before pushing:

    qmk lint -kb splitkb/aurora/lily58/rev1 -km laureyn --strict
