import qualified Data.Word as W
import qualified Data.ByteString as B
import qualified Data.Primitive.ByteArray as BA

data Chip8 = Chip8 {
    pc :: W.Word16
    sp :: W.Word16
    regs :: B.ByteString
    memory :: B
}

program_start = 0x200
program_end = 0xe90

c8 = Chip8 {
    pc = program_start
    sp = program_start
}
