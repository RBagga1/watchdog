import tkinter as tk
import enum
import argparse
import sys

ERROR_BG_COLOR = "#ebbaba"
ERROR_FG_COLOR = "#000000"
SUCCESS_BG_COLOR = "#83c983"
SUCCESS_FG_COLOR = "#000000"
WINDOW_WIDTH = 400
WINDOW_HEIGHT = 120
DISPLAY_TIME_SECONDS = 5
PADDING = 20


class Position(enum.Enum):
    TOP_LEFT = 0
    TOP_RIGHT = 1
    BOTTOM_LEFT = 2
    BOTTOM_RIGHT = 3


def format_msg(status: str, message: str, exit_code: int) -> str:
    """
    Formats the notification message.
    """
    return (
        f"{'Watchdog: ' + status.upper()}\nExit Code: {exit_code}\nstdout:\n\n{message}"
    )


def show_notification(
    title: str,
    message: str,
    exit_code: int,
    status="success",
    pos: Position = Position.BOTTOM_RIGHT,
) -> None:
    """
    Displays a notification with sound and color depending on command execution status.

    :param title: The title of the notification.
    :param message: The message content of the notification.
    :param status: The status of the command execution (success or error).
    """
    root = tk.Tk()
    root.withdraw()
    root.title(title)

    # Force update of state to get accurate dimensions
    root.update_idletasks()

    if status.lower() == "error":
        bg_color = ERROR_BG_COLOR
        fg_color = ERROR_FG_COLOR
    else:
        bg_color = SUCCESS_BG_COLOR
        fg_color = SUCCESS_FG_COLOR

    root.configure(bg=bg_color)

    # Play default sys alert sound
    root.bell()

    # Position window and style it
    screen_width = root.winfo_screenwidth()
    screen_height = root.winfo_screenheight()

    if pos == Position.TOP_LEFT:
        x_pos = PADDING
        y_pos = PADDING
    elif pos == Position.TOP_RIGHT:
        x_pos = screen_width - WINDOW_WIDTH - PADDING
        y_pos = PADDING
    elif pos == Position.BOTTOM_LEFT:
        x_pos = PADDING
        y_pos = screen_height - WINDOW_HEIGHT - PADDING
    else:  # Position.BOTTOM_RIGHT as default
        x_pos = screen_width - WINDOW_WIDTH - PADDING
        y_pos = screen_height - WINDOW_HEIGHT - PADDING

    root.geometry(f"{WINDOW_WIDTH}x{WINDOW_HEIGHT}+{x_pos}+{y_pos}")
    root.attributes("-topmost", True)

    # Create and style the label
    label = tk.Label(
        root,
        text=format_msg(status, message, exit_code),
        bg=bg_color,
        fg=fg_color,
        font=("Arial", 10),
        padx=15,
        pady=15,
        wraplength=WINDOW_WIDTH - 30,
        justify=tk.LEFT,
    )
    label.pack(expand=True, fill=tk.BOTH)

    # Show window
    root.deiconify()
    root.after(DISPLAY_TIME_SECONDS * 1000, root.destroy)
    root.mainloop()


def main():
    pass


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Show a notification for the output of a command."
    )
    parser.add_argument(
        "-t", "--title", type=str, help="Title of the notification", required=True
    )
    parser.add_argument(
        "-m",
        "--message",
        type=str,
        help="Message content of the notification",
        required=True,
    )
    parser.add_argument(
        "-e", "--exit_code", type=int, help="Exit code of the command", required=True
    )
    parser.add_argument(
        "-s",
        "--status",
        type=str,
        choices=["success", "error"],
        help="Status of the command execution",
        required=True,
    )

    try:
        args = parser.parse_args()
    except Exception as e:
        print(f"Error parsing arguments: {e}")
        sys.exit(1)

    show_notification(args.title, args.message, args.exit_code, args.status)
