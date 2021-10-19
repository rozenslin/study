package wahoo
import "fmt"
import "errors"

func Double_msg(msg string) (string, error) {
    if msg == "" {
        return msg, errors.New("Invalid string")
    }

    return fmt.Sprintf("%s:%s", msg, msg), nil
}
