# Implement framework and plugin

## Introduce the plugin and the interface.

Interface is a calling convention or a calling policy between program modules.
The cstring example is showing the example of the interface and plugin.

![interface and plug-in of cstring](/interface.png)

그림에서 보는 것과 같이 cstring 구조체에 저장된 함수 포인터가 인터페이스입니다.
* cstring.clear
* cstring.at

인터페이스는 플러그인에서 특정한 틀을 제공합니다. 플러그인은 이 틀에 맞춰서 서비스를 제공해야합니다.

cstring.c 파일에 구현된 각 함수들이 바로 플러그인이 됩니다.
* cstring_clear
* cstring_at

다시 설명하면 cstring 구조체에있는 함수 포인터들은 cstring이라는 계층 혹은 모듈이 어떤 인터페이스를 main함수에 제공할지를 알려주는 역할을 합니다. main함수는 cstring 구조체에있는 함수 포인터만을 보고 함수들을 호출할 수 있습니다.

cstring.c에는 각 함수들이 구현되어있습니다. 이 것들이 플러그인입니다. main함수에서는 cstring.c에있는 플러그인이 보이지도않고 접근할 수도 없습니다. 오직 cstring 구조체를 통해서만 플러그인에 접근할 수 있습니다.

플러그인은 얼마든지 변할 수 있습니다. 누군가가 좀더 성능이 좋은 cstring을 만들수 있다면 cstring.c라 아니라 cstring2.c를 만들어서 cstring.c를 대체할 수 있습니다. cstring2.c에 각 함수들의 구현을 만들고 create_cstring 함수를 만들기만하면 main함수에서는 내가 호출할 cstring.clear 함수가 cstring.c에서 구현된 것인지 cstring2.c에서 구현된 것인지 알 수도 없고 알 필요도 없습니다.

왜 cstring을 구현해봤는지 이해가 되시나요? 단순히 구조체안에 함수 포인터를 넣을 수 있다거나, C++의 클래스를 흉내낼 수 있다는 것이 전부가 아닙니다. 프로그램을 만들 때 어떤 부분은 인터페이스가 되고, 어떤 부분은 플러그인이 되어야하는지를 구별해서 구현해야한다는걸 보여드리기 위해서입니다.

만약 처음 인터페이스 개념을 접하셨다면 아직 잘 이해가 안될 것입니다. 다음으로 cstring을 테스트할 테스트 프레임웍을 만들면서 좀더 인터페이스와 플러그인 개념을 알아보겠습니다.
