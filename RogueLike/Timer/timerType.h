#pragma once

//deltaTime으로 사용할 Type을 정의한다.
//사용 가능한 타입은 부동소수점 자료형만 가능.
using deltaTimeType = float;
//운영체제의 시간이 변경되더라도 게임의 시간이 뒤로 가는일이 존재해서는 안된다.
using steadyTimer = std::chrono::steady_clock;